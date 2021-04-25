#pragma once
#include "common/staticMesh3D.h"

namespace static_meshes_3D {

	/**
	* Cylinder static mesh with given radius, number of slices and height.
	*/
	class Cone : public StaticMesh3D
	{
	public:
		Cone(float radius, int numSlices, float height,
			bool withPositions = true, bool withTextureCoordinates = true, bool withNormals = true);

		void render() const override;
		void renderPoints() const override;

		/**
		 * Gets Cone radius.
		 */
		float getRadius() const;

		/**
		 * Gets number of Coner slices.
		 */
		int getSlices() const;

		/**
		 * Gets Cone height.
		 */
		float getHeight() const;

	private:
		float _radius; //Coneradius (distance from the center of cylinder to surface)
		int _numSlices; // Number of Cone slices
		float _height; // Height of theCone

		int _numVerticesSide; // How many vertices to render side of the cCone
		int _numVerticesTopBottom; // How many vertices to render top / bottom of the cCone
		int _numVerticesTotal; // Just a sum of both numbers above

		void initializeData() override;
	};

} // namespace static_meshes_3D