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
Vector3 EnvironmentLight::GetDirectionFromPositionScaled(const ShadingInfo& shadingInfo) const {
	Point3 sp = sampler->GetSampleOnHemisphere();
	Vector3 right;
	Vector3 up(0.0034f, 1.0f, 0.0071f);
	Vector3 forward = shadingInfo.normalVector;
	CommonMath::ComputeUVWFromWandU(right, up, forward);
	// negate because this is light coming inwards, not out
	return -right*sp[0] - up*sp[1] - forward*sp[2];
}

Color3 EnvironmentLight::GetRadiance(ShadingInfo& shadingInfo, const Scene& scene) const {
	Color areaLightColor = material->GetColorForAreaLight(shadingInfo);
	return Color3(areaLightColor[0], areaLightColor[1],
				  areaLightColor[2]);
}
