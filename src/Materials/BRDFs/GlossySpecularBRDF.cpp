#include "GlossySpecularBRDF.h"
#include "Sampling/GenericSampler.h"
#include "Math/CommonMath.h"

GlossySpecularBRDF::GlossySpecularBRDF() : sampler(nullptr), ks(0), cs(nullptr), exponent(0.0f) {
}
	
GlossySpecularBRDF::GlossySpecularBRDF(GenericSampler *sampler, float ks,
									   std::shared_ptr<AbstractTexture> const &cs,
									   float exponent) : sampler(sampler), ks(ks),
	cs(cs), exponent(exponent) {
	sampler->MapSamplesToHemisphere(exponent);
}

GlossySpecularBRDF::~GlossySpecularBRDF() {
	if (sampler != nullptr) {
		delete sampler;
	}
}

void GlossySpecularBRDF::setSampler(GenericSampler *sampler) {
	if (this->sampler != nullptr) {
		delete sampler;
	}
	this->sampler = sampler;
	this->sampler->MapSamplesToHemisphere(exponent);
}

Vector3 GlossySpecularBRDF::GetReflectionVector(Vector3 const & wo,
							Vector3 const & normal) const {
	float ndotwo = normal * wo;
	return (-wo + normal * ndotwo * 2.0f).Normalized();
}

Color3 GlossySpecularBRDF::F(ShadingInfo const & shadingInfo) const {
	Color3 	finalColor;
	Vector3 const & normal = shadingInfo.normalVector;
	Vector3 reflectedVector = GetReflectionVector(shadingInfo.wi, normal);
	Vector3 const & outgoingDir = shadingInfo.wo;
	float 		rDotOutgoing = reflectedVector * outgoingDir;
		
	// highlight is strongest when outgoing aligns with reflection
	if (rDotOutgoing > 0.0) {
		finalColor = cs->GetColor(shadingInfo) * ks * pow(rDotOutgoing, exponent);
	}
			
	return finalColor;
}

Vector3 GlossySpecularBRDF::GetReflectionVectorSampled(Vector3 const & wo,
								   Vector3 const & normal, float& rIncomingDotNormal) const {
	Vector3 reflectedVector = GetReflectionVector(wo, normal);
	
	Vector3 w = reflectedVector;
	Vector3 u = Vector3(0.00424f, 1.0f, 0.00764f) ^ w;
	u.Normalize();
	Vector3 v = w ^ u;
	v.Normalize();
	CommonMath::ComputeUVWFromUpandForward(u, v, w);
	
	// find a sample in coordinate system centered around reflection vector
	// that's your new reflection vector
	Point3 samplePoint = sampler->GetSampleOnHemisphere();
	reflectedVector = u*samplePoint[0] + v*samplePoint[1] + w*samplePoint[2];
	
	rIncomingDotNormal = reflectedVector*normal;
	
	if (rIncomingDotNormal < 0.0f) {
		reflectedVector = -u*samplePoint[0] - v*samplePoint[1] + w*samplePoint[2];
	}
	
	return reflectedVector.Normalized();
}

Color3 GlossySpecularBRDF::SampleF(ShadingInfo const & shadingInfo, float& pdf, Vector3 &newWi) const {
	Color3 	finalColor;
	// for indirect illumination
	Vector3 const & normal = shadingInfo.normalVector;
	float rIncomingDotNormal;
	Vector3 reflectedVector = GetReflectionVectorSampled(shadingInfo.wi, normal,
														 rIncomingDotNormal);
	
	float phongLobe = pow(reflectedVector*newWi,
						  exponent);
	pdf = rIncomingDotNormal * phongLobe;
			
	return cs->GetColor(shadingInfo) * ks * phongLobe;
}

Color3 GlossySpecularBRDF::GetRho(const ShadingInfo& shadingInfo) const {
	return Color3::Black();
}

