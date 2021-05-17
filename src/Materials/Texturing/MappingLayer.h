#pragma once

class MappingLayer {
public:
	virtual ~MappingLayer() {
		
	}
	
	virtual void ComputeTextureCoordinates(ShadingInfo const & shadingInfo,
										   int width, int height,
										   int & row, int & column) = 0;

private:

};

