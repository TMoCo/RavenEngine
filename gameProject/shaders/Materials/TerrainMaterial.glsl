




void ComputeMaterial(in MaterialData inData, out MaterialOutput outParams)
{
	outParams.color = vec3(1.0, 1.0, 1.0);
	outParams.emission = vec3(0.0);
	outParams.roughness = 1.0;
	outParams.metallic = 0.0;
	outParams.specular = 1.0;
	outParams.normal = inData.normal;
}

