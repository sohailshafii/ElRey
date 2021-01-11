#include "GlossySpecularBRDF.h"

GlossySpecularBRDF::GlossySpecularBRDF() : ks(0), cs(Color3(0.0f, 0.0f, 0.0f)),
	csScaled(cs*ks) {
}
	
GlossySpecularBRDF::GlossySpecularBRDF(float ks, Color3 cs, float exponent) {
	this->ks = ks;
	this->cs = cs;
	this->csScaled = cs*ks;
	this->exponent = exponent;
}

Color3 GlossySpecularBRDF::GetRadiance(const IntersectionResult& intersectionResult) const {
	Color3 	finalColor;
	Vector3 const & intersectionNormal = intersectionResult.normalVector;
	Vector3 const & incomingDirReverse = intersectionResult.incomingDirInverse;
	float 		nDotIncomingDirection = intersectionNormal * incomingDirReverse;
	Vector3 	reflectedVector(-incomingDirReverse + intersectionNormal * nDotIncomingDirection * 2.0f);
	Vector3 const & outgoingDir = intersectionResult.vectorToLight;
	float 		rDotOutgoing = reflectedVector * outgoingDir;
		
	if (rDotOutgoing > 0.0) {
		finalColor = csScaled * pow(rDotOutgoing, exponent);
	}
			
	return finalColor;
}

Color3 GlossySpecularBRDF::GetRho(const IntersectionResult& IntersectionResult) const {
	return Color3::Black();
}

