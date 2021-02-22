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
	Vector3 const & lightVector = shadingInfo.vectorToLight;
	float 		nDotIncomingLightVec = intersectionNormal * lightVector;
	Vector3 	reflectedVector(-lightVector + intersectionNormal * nDotIncomingLightVec * 2.0f);
	Vector3 const & outgoingDir = shadingInfo.incomingDirInverse;
	float 		rDotOutgoing = reflectedVector * outgoingDir;
		
	if (rDotOutgoing > 0.0) {
		finalColor = csScaled * pow(rDotOutgoing, exponent);
	}
			
	return finalColor;
}

Color3 GlossySpecularBRDF::SampleF(ShadingInfo& shadingInfo, float& pdf) const {
	Color3 	finalColor;
	Vector3 const & intersectionNormal = shadingInfo.normalVector;
	Vector3 const & incomingDirReverse = shadingInfo.incomingDirInverse;
	float 		nDotIncomingDirection = intersectionNormal * incomingDirReverse;
	Vector3 	reflectedVector(-incomingDirReverse + intersectionNormal * nDotIncomingDirection * 2.0f);
	
	Vector3 w = reflectedVector;
	Vector3 u = Vector3(0.00424, 1, 0.00764) ^ w;
	u.Normalize();
	Vector3 v = u ^ w;
	
	Point3 samplePoint = sampler->GetSampleOnHemisphere();
	shadingInfo.incomingDirInverse =
		u*samplePoint[0] + v*samplePoint[1] + w*samplePoint[2];
	
	float rIncomingDotNormal = shadingInfo.incomingDirInverse*
		intersectionNormal;
	
	if (rIncomingDotNormal < 0.0f) {
		shadingInfo.incomingDirInverse =
			-u*samplePoint[0] - v*samplePoint[1] - w*samplePoint[2];
	}
	
	shadingInfo.incomingDirInverse.Normalize();
	shadingInfo.rayDirection = -shadingInfo.incomingDirInverse;
	
	float phongLobe = pow(
			reflectedVector*shadingInfo.incomingDirInverse,
						  exponent);
	pdf = rIncomingDotNormal * phongLobe;
			
	return csScaled * phongLobe;
}

Color3 GlossySpecularBRDF::GetRho(const ShadingInfo& shadingInfo) const {
	return Color3::Black();
}

