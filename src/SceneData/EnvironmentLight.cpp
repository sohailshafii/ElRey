#include "EnvironmentLight.h"
#include "Math/CommonMath.h"

EnvironmentLight::EnvironmentLight(bool castsShadows,
								   RandomSamplerType randomSamplerType,
								   unsigned int numRandomSamples,
								   unsigned int numRandomSets,
								   std::shared_ptr<Material> const& iMaterial) :
	Light(castsShadows) {
	sampler = SamplerCreator::CreatorSampler(randomSamplerType, numRandomSamples, numRandomSets);
	sampler->MapSamplesToHemisphere(1.0f);
	material = iMaterial;
}

EnvironmentLight::~EnvironmentLight() {
	if (sampler != nullptr) {
		delete sampler;
	}
}

// when tracing to area light, need to get position
// via sampling. same goes with normal values
Vector3 EnvironmentLight::GetDirectionFromPositionScaled(
	const IntersectionResult& intersectionRes) const {
	Point3 sp = sampler->GetSampleOnHemisphere();
	Vector3 right;
	Vector3 up(0.0034f, 1.0f, 0.0071f);
	Vector3 forward = intersectionRes.GetNormalVector();
	CommonMath::ComputeUVWFromWandU(right, up, forward);
	// negate because this is light coming inwards, not out
	return -right*sp[0] - up*sp[1] - forward*sp[2];
}

Color3 EnvironmentLight::GetRadiance(const IntersectionResult& intersectionRes, const Scene& scene) {
	Color areaLightColor = material->GetColorForAreaLight(intersectionRes);
	return Color3(areaLightColor[0], areaLightColor[1],
				  areaLightColor[2]);
}
