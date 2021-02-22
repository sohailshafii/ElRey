#include "GlossySpecularBRDF.h"
#include "Sampling/GenericSampler.h"

GlossySpecularBRDF::GlossySpecularBRDF() : sampler(nullptr), ks(0), cs(Color3(0.0f, 0.0f, 0.0f)), csScaled(cs*ks), exponent(0.0f) {
}
	
GlossySpecularBRDF::GlossySpecularBRDF(GenericSampler *sampler, float ks, Color3 cs, float exponent) : sampler(sampler), ks(ks),
	cs(cs), csScaled(cs*ks), exponent(exponent) {
	sampler->MapSamplesToHemisphere(exponent);
}

GlossySpecularBRDF::~GlossySpecularBRDF() {
	if (sampler != nullptr) {
		delete sampler;
	}
}

Color3 GlossySpecularBRDF::F(ShadingInfo& shadingInfo) const {
	Color3 	finalColor;
	Vector3 const & intersectionNormal = shadingInfo.normalVector;
	Vector3 const & wi = shadingInfo.wi;
	float 		nDotIncomingLightVec = intersectionNormal * wi;
	Vector3 	reflectedVector(-wi +
								intersectionNormal * nDotIncomingLightVec * 2.0f);
	Vector3 const & outgoingDir = shadingInfo.wo;
	float 		rDotOutgoing = reflectedVector * outgoingDir;
		
	if (rDotOutgoing > 0.0) {
		finalColor = csScaled * pow(rDotOutgoing, exponent);
	}
			
	return finalColor;
}

Color3 GlossySpecularBRDF::SampleF(ShadingInfo& shadingInfo, float& pdf) const {
	Color3 	finalColor;
	Vector3 const & intersectionNormal = shadingInfo.normalVector;
	Vector3 const & wo = shadingInfo.wo;
	float 		nDotIncomingDirection = intersectionNormal * wo;
	Vector3 	reflectedVector(-wo +
								intersectionNormal * nDotIncomingDirection * 2.0f);
	
	Vector3 w = reflectedVector;
	Vector3 u = Vector3(0.00424, 1, 0.00764) ^ w;
	u.Normalize();
	Vector3 v = u ^ w;
	
	Point3 samplePoint = sampler->GetSampleOnHemisphere();
	shadingInfo.wi =
		u*samplePoint[0] + v*samplePoint[1] + w*samplePoint[2];
	
	float rIncomingDotNormal = shadingInfo.wi*intersectionNormal;
	
	if (rIncomingDotNormal < 0.0f) {
		shadingInfo.wi =
			-u*samplePoint[0] - v*samplePoint[1] - w*samplePoint[2];
	}
	
	shadingInfo.wi.Normalize();
	
	float phongLobe = pow(reflectedVector*shadingInfo.wi,
						  exponent);
	pdf = rIncomingDotNormal * phongLobe;
			
	return csScaled * phongLobe;
}

Color3 GlossySpecularBRDF::GetRho(const ShadingInfo& shadingInfo) const {
	return Color3::Black();
}

