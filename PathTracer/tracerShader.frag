// Comment to remove
#define BOUNCE_SHADOW_TEST

uniform samplerBuffer treeBuffer;
uniform samplerBuffer triBuffer;
uniform samplerBuffer materialBuffer;

uniform float randSeed;

uniform vec3 cameraDir;
uniform vec2 cameraDims;
uniform vec3 cameraPosition;
uniform vec4 cameraRotation;

const int maxKDSteps = 600;
const int maxNumLights = 8;
uniform int numLights;
uniform vec3 lightPositions[maxNumLights];
uniform vec3 lightColors[maxNumLights];

const vec3 attenuation = vec3(5.0, 0.0004, 0.000001);

const int numAdditionalBounces = 1;
const int numPathSamples = 1;
const int numShadowSamples = 1;

const float testOffset = 0.01;

const float lightJitterDist = 12.0;

const float queryOffset = 0.01;

vec3 Hash3(vec2 n)
{
	return fract(sin(vec3(n.x, n.y, n + 2.0)) * vec3(3.5453123, 4.1459123, 1.3490423));
}

float Rand(vec2 co)
{
    return fract(sin(dot(co.xy + vec2(randSeed), vec2(12.9898, 78.2331))) * 43758.5453) - 0.5;
}

vec3 Rand3(vec2 co)
{
	return vec3(Rand(co), Rand(co + vec2(16.2328, 97.3211)), Rand(co - vec2(16.2328, 97.3211)));
}

bool IntersectsTriangle(vec3 p1, vec3 p2, vec3 p3, vec3 start, vec3 dir, out float t)
{
	vec3 e1 = p2 - p1;
	vec3 e2 = p3 - p1;

	vec3 h = cross(dir, e2);
	float a = dot(e1, h);

	//if(abs(a) < 0.00001)
	//	return false;

	float f = 1.0 / a;

	vec3 s = start - p1;

	float u = f * dot(s, h);

	if(u < 0.0 || u > 1.0)
		return false;

	vec3 q = cross(s, e1);

	float v = f * dot(dir, q);

	if(v < 0.0 || u + v > 1.0)
		return false;

	t = f * dot(e2, q);

	if(t > 0.00001)
		return true;

	return false;
}

vec3 GetBarycentricCoords(vec3 p1, vec3 p2, vec3 p3, vec3 hitPos)
{
	// Find barycentric coordinates (area-weighted coordinates of hitPos)
	vec3 f0 = p1 - hitPos;
	vec3 f1 = p2 - hitPos;
	vec3 f2 = p3 - hitPos;

	vec3 vecArea = cross(p1 - p2, p1 - p3);
	vec3 vecArea0 = cross(f1, f2);
	vec3 vecArea1 = cross(f2, f0);
	vec3 vecArea2 = cross(f0, f1);

	float areaInv = 1.0f / length(vecArea);
	float area0 = length(vecArea0) * areaInv * (dot(vecArea, vecArea0) > 0.0f ? 1.0 : -1.0);
	float area1 = length(vecArea1) * areaInv * (dot(vecArea, vecArea1) > 0.0f ? 1.0 : -1.0);
	float area2 = length(vecArea2) * areaInv * (dot(vecArea, vecArea2) > 0.0f ? 1.0 : -1.0);

	return vec3(area0, area1, area2);
}

bool AABBContainsPoint(vec3 lower, vec3 upper, vec3 p)
{
	if(p.x < lower.x || p.y < lower.y || p.z < lower.z || p.x > upper.x || p.y > upper.y || p.z > upper.z)
		return false;

	return true;
}

bool AABBIntersectionPoints(vec3 start, vec3 direction, vec3 min, vec3 max, out float t[2])
{
	vec3 center = (max + min) * 0.5;
	vec3 halfDims = (max - min) * 0.5;

	bool parallel[3] = bool[3](false, false, false);
	bool hasParallel = false;

	bool found = false;

	const float epsilon = 0.01;

	// Array representing vector so it is easier to parse
	float S[3] = float[3](start.x, start.y, start.z);
	float D[3] = float[3](direction.x, direction.y, direction.z);
	float d[3] = float[3](center.x - start.x, center.y - start.y, center.z - start.z);
	float e[3] = float[3](halfDims.x, halfDims.y, halfDims.z);

	for(int i = 0; i < 3; i++)
	{
		float es = D[i] > 0.0 ? e[i] : -e[i];

		float invD = 1.0 / D[i];

		if(!found)
		{
			t[0] = (d[i] - es) * invD;
			t[1] = (d[i] + es) * invD;

			found = true;
		}
		else
		{
			float s = (d[i] - es) * invD;

			if(s > t[0])
				t[0] = s;

			s = (d[i] + es) * invD;

			if(s < t[1])
				t[1] = s;

			if(t[0] > t[1])
				return false;
		}
	}

	/*if(hasParallel)
		for(int i = 0; i < 3; i++)
			if(parallel[i])
				if(abs(d[i] - t[0] * D[i]) > e[i] || abs(d[i] - t[1] * D[i]) > e[i])
					return false;*/

	return true;
}

bool RayCast(vec3 start, vec3 dir, out vec3 hitPos, out vec3 hitNormal, out float materialIndex)
{
	float minT = 99999.0;
	int minTriangleIndex;

	// Find initial query point
	vec3 currentLower = texelFetchBuffer(treeBuffer, 0).xyz;
	vec3 currentUpper = texelFetchBuffer(treeBuffer, 1).xyz;

	float rootT[2];

	if(!AABBIntersectionPoints(start, -dir, currentLower, currentUpper, rootT))
		return false;

	vec3 queryPoint = start - dir * (rootT[0] + testOffset);

	int currentNodeStart = 0;

	bool hit = false;

	// Safety counter
	for(int s = 0; s < maxKDSteps; s++)
	{
		vec3 children = texelFetchBuffer(treeBuffer, currentNodeStart + 3).xyz;

		// Go through child containing the query point if it has children. If not (and no triangles were previously hit), find a new query point
		if(children.x > 0.0)
		{
			int childNodeIndex1 = int(children.x);
			int childNodeStart1 = childNodeIndex1 * 4;

			vec3 lower1 = texelFetchBuffer(treeBuffer, childNodeStart1).xyz;
			vec3 upper1 = texelFetchBuffer(treeBuffer, childNodeStart1 + 1).xyz;

			if(AABBContainsPoint(lower1, upper1, queryPoint))
			{
				// Continue search from this node
				currentNodeStart = childNodeStart1;
				currentLower = lower1;
				currentUpper = upper1;
			}
			else
			{
				int childNodeIndex2 = int(children.y);
				int childNodeStart2 = childNodeIndex2 * 4;

				vec3 lower2 = texelFetchBuffer(treeBuffer, childNodeStart2).xyz;
				vec3 upper2 = texelFetchBuffer(treeBuffer, childNodeStart2 + 1).xyz;

				if(AABBContainsPoint(lower2, upper2, queryPoint))
				{
					// Continue search from this node
					currentNodeStart = childNodeStart2;
					currentLower = lower2;
					currentUpper = upper2;
				}
				else
					break;
			}
		}
		else
		{
			vec3 geom = texelFetchBuffer(treeBuffer, currentNodeStart + 2).xyz;

			// Query all triangles in current node
			int geomStartIndex = int(geom.x);
			int numTris = int(geom.y);

			for(int i = 0; i < numTris; i++)
			{
				int j = (geomStartIndex + i) * 9;

				vec3 pt1 = texelFetchBuffer(triBuffer, j).xyz;
				vec3 pt2 = texelFetchBuffer(triBuffer, j + 3).xyz;
				vec3 pt3 = texelFetchBuffer(triBuffer, j + 6).xyz;

				float t;

				if(IntersectsTriangle(pt1, pt2, pt3, start, dir, t))
				{
					if(t < minT)
					{
						minT = t;
						minTriangleIndex = j;
					}

					hit = true;
				}
			}

			if(hit)
				break;

			// Still no hits, continue search
			// Ray cast aabb to find new point
			float ct[2];

			AABBIntersectionPoints(start, dir, currentLower, currentUpper, ct);

			queryPoint = start + dir * (ct[1] + queryOffset);

			// Go up to continue search
			currentNodeStart = 0; // Each level is 4, go up a few levels
		}
	}

	if(hit)
	{
		hitPos = start + dir * minT;

		vec3 p1 = texelFetchBuffer(triBuffer, minTriangleIndex).xyz;
		vec3 p2 = texelFetchBuffer(triBuffer, minTriangleIndex + 3).xyz;
		vec3 p3 = texelFetchBuffer(triBuffer, minTriangleIndex + 6).xyz;

		vec3 bary = GetBarycentricCoords(p1, p2, p3, hitPos);

		hitNormal = 
			texelFetchBuffer(triBuffer, minTriangleIndex + 1).xyz * bary.x + 
			texelFetchBuffer(triBuffer, minTriangleIndex + 4).xyz * bary.y + 
			texelFetchBuffer(triBuffer, minTriangleIndex + 7).xyz * bary.z;

		materialIndex = texelFetchBuffer(triBuffer, minTriangleIndex + 2).z;
	}

	return hit;
}

bool RayCast_Shadow(vec3 start, vec3 dir, float refDistance)
{
	// Find initial query point
	vec3 currentLower = texelFetchBuffer(treeBuffer, 0).xyz;
	vec3 currentUpper = texelFetchBuffer(treeBuffer, 1).xyz;

	float rootT[2];

	if(!AABBIntersectionPoints(start, dir, currentLower, currentUpper, rootT))
		return false;

	vec3 queryPoint = start + dir * (rootT[0] + testOffset);

	int currentNodeStart = 0;

	// Safety counter
	for(int s = 0; s < maxKDSteps; s++)
	{
		vec3 children = texelFetchBuffer(treeBuffer, currentNodeStart + 3).xyz;

		// Go through child containing the query point if it has children. If not (and no triangles were previously hit), find a new query point
		if(children.x > 0.0)
		{
			int childNodeIndex1 = int(children.x);
			int childNodeStart1 = childNodeIndex1 * 4;

			vec3 lower1 = texelFetchBuffer(treeBuffer, childNodeStart1).xyz;
			vec3 upper1 = texelFetchBuffer(treeBuffer, childNodeStart1 + 1).xyz;

			if(AABBContainsPoint(lower1, upper1, queryPoint))
			{
				// Continue search from this node
				currentNodeStart = childNodeStart1;
				currentLower = lower1;
				currentUpper = upper1;
			}
			else
			{
				int childNodeIndex2 = int(children.y);
				int childNodeStart2 = childNodeIndex2 * 4;

				vec3 lower2 = texelFetchBuffer(treeBuffer, childNodeStart2).xyz;
				vec3 upper2 = texelFetchBuffer(treeBuffer, childNodeStart2 + 1).xyz;

				if(AABBContainsPoint(lower2, upper2, queryPoint))
				{
					// Continue search from this node
					currentNodeStart = childNodeStart2;
					currentLower = lower2;
					currentUpper = upper2;
				}
				else
					return false;
			}
		}
		else
		{
			vec3 geom = texelFetchBuffer(treeBuffer, currentNodeStart + 2).xyz;

			// Query all triangles in current node
			int geomStartIndex = int(geom.x);
			int numTris = int(geom.y);

			for(int i = 0; i < numTris; i++)
			{
				int j = (geomStartIndex + i) * 9;

				vec3 pt1 = texelFetchBuffer(triBuffer, j).xyz;
				vec3 pt2 = texelFetchBuffer(triBuffer, j + 3).xyz;
				vec3 pt3 = texelFetchBuffer(triBuffer, j + 6).xyz;

				float t;

				if(IntersectsTriangle(pt1, pt2, pt3, start, dir, t))
					if(t < refDistance)
						return true;
			}

			// Still no hits, continue search
			// Ray cast aabb to find new point
			float ct[2];

			AABBIntersectionPoints(start, dir, currentLower, currentUpper, ct);

			queryPoint = start + dir * (ct[1] + queryOffset);

			currentNodeStart = 0;
		}
	}

	return false;
}

vec3 QuaternionTransform(vec4 quat, vec3 vec)
{
	vec3 quatVec = vec3(quat.x, quat.y, quat.z);
	vec3 uv = vec3(cross(quatVec, vec));
	vec3 uuv = vec3(cross(quatVec, uv));
	uv *= 2.0 * quat.w;
	uuv *= 2.0;

	return vec + uv + uuv;
}

void main()
{
	float dX = (gl_TexCoord[0].s - 0.5) * cameraDims.x;
	float dY = (gl_TexCoord[0].t - 0.5) * cameraDims.y;

	vec3 rayStart = QuaternionTransform(cameraRotation, vec3(dX, dY, 0.0)) + cameraPosition;

	vec3 hitPos;
	vec3 hitNormal;
	float materialIndex;

	if(RayCast(rayStart, cameraDir, hitPos, hitNormal, materialIndex))
	{
		int materialStartIndex = int(materialIndex) * 2;

		vec3 diffuseColor = texelFetchBuffer(materialBuffer, materialStartIndex).xyz;
		vec3 materialData = texelFetchBuffer(materialBuffer, materialStartIndex + 1).xyz;

		vec3 lightColor = vec3(0.0, 0.0, 0.0);

		for(int s = 0; s < numShadowSamples; s++)
		{
			for(int i = 0; i < numLights; i++)
			{
				vec3 offsetLightPosition = lightPositions[i] + lightJitterDist * normalize(Rand3(gl_TexCoord[0].st + vec2(i * 231.0 + s * 767.0)));
				vec3 toLight = offsetLightPosition - hitPos;
				float dist = length(toLight);
				toLight /= dist;

				if(!RayCast_Shadow(offsetLightPosition, -toLight, dist - testOffset))
					lightColor += max(0.0, dot(toLight, hitNormal)) * lightColors[i] * (1.0 / (attenuation.x + dist * attenuation.y + dist * dist * attenuation.z)); 
			}
		}

		lightColor /= numShadowSamples;

		vec3 firstBounceColor = lightColor * diffuseColor;
		vec3 color = vec3(0.0, 0.0, 0.0);

		vec3 reflected = reflect(cameraDir, hitNormal);

		// Samples
		for(int i = 0; i < numPathSamples; i++)
		{
			vec3 prevColor = diffuseColor;
			vec3 subColor = firstBounceColor;

			vec3 bounceHitPos;
			vec3 bounceHitNormal;
			float bounceMaterialIndex;

			vec3 bounceDiffuseColor = diffuseColor;
			vec3 bounceMaterialData = materialData;

			float bounceInfluence = bounceMaterialData.x;

			for(int j = 0; j < numAdditionalBounces; j++)
			{
				vec3 dir = normalize(reflected + normalize(Rand3(gl_TexCoord[0].st + vec2(j * 43.0 + i * 195.0))) * (1.0 - bounceMaterialData.y));

				if(RayCast(hitPos + hitNormal * testOffset, dir, bounceHitPos, bounceHitNormal, bounceMaterialIndex))
				{
					vec3 lightColor = vec3(0.0, 0.0, 0.0);

					for(int l = 0; l < numLights; l++)
					{
						vec3 offsetLightPosition = lightPositions[l] + lightJitterDist * normalize(Rand3(gl_TexCoord[0].st + vec2(i * 23.0 + j * 76.0 + l * 195.0)));
						vec3 toLight = offsetLightPosition - bounceHitPos;
						float dist = length(toLight);
						toLight /= dist;

#ifdef BOUNCE_SHADOW_TEST
						if(!RayCast_Shadow(offsetLightPosition, -toLight, dist - testOffset))
#endif
							lightColor += max(0.0, dot(toLight, bounceHitNormal)) * lightColors[l] * (1.0 / (attenuation.x + dist * attenuation.y + dist * dist * attenuation.z)); 
					}

					int bounceMaterialStartIndex = int(bounceMaterialIndex) * 2;

					prevColor = bounceDiffuseColor;

					bounceDiffuseColor = texelFetchBuffer(materialBuffer, bounceMaterialStartIndex).xyz;
					bounceMaterialData = texelFetchBuffer(materialBuffer, bounceMaterialStartIndex + 1).xyz;

					subColor += lightColor * bounceDiffuseColor * prevColor * bounceInfluence;

					bounceInfluence *= bounceMaterialData.x;

					hitPos = bounceHitPos;
					hitNormal = bounceHitNormal;

					reflected = reflect(dir, bounceHitNormal);
				}
				else
					break;
			}

			color += subColor;
		}

		color /= (numPathSamples + 1);

		gl_FragColor = vec4(color, 0.0);
	}
	else
		gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
}
