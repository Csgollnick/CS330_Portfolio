#pragma once
#include "common/staticMesh3D.h"

namespace static_meshes_3D {

	/**
	* OpenCylinder static mesh with given radius, number of slices and height.
	*/
	class OpenCylinder : public StaticMesh3D
	{
	public:
		OpenCylinder(float radius, int numSlices, float height,
			bool withPositions = true, bool withNormals = true, bool withTextureCoordinates = true);

		void render() const override;
		void renderPoints() const override;

		/**
		 * Gets OpenCylinder radius.
		 */
		float getRadius() const;

		/**
		 * Gets number of OpenCylinder slices.
		 */
		int getSlices() const;

		/**
		 * Gets OpenCylinder height.
		 */
		float getHeight() const;

	private:
		float _radius; // OpenCylinder radius (distance from the center of OpenCylinder to surface)
		int _numSlices; // Number of OpenCylinder slices
		float _height; // Height of the OpenCylinder

		int _numVerticesSide; // How many vertices to render side of the OpenCylinder
		int _numVerticesTopBottom; // How many vertices to render top / bottom of the OpenCylinder
		int _numVerticesTotal; // Just a sum of both numbers above

		void initializeData() override;
	};

} // namespace static_meshes_3D