/*

// material properties
#define AMBIENT_COEFF 			0.1f
#define DIFFUSE_COEFF 			0.6f
#define SPECULAR_COEFF 			0.3f
#define SHINY_COEFF 			50.0f

#define MATERIAL_COLOUR 		float4(1.0f, 0.0f, 0.0f, 1.0f)

// light properties
#define AMBIENT_LIGHT_COLOUR 	float3(1.0f, 1.0f, 1.0f)
#define DIFFUSE_LIGHT_COLOUR 	float3(1.0f, 1.0f, 1.0f)
#define SPECULAR_LIGHT_COLOUR 	float3(1.0f, 1.0f, 1.0f)

#define LIGHT_POS				float3(100.0f, 100.0f, 100.0f)

cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
}

cbuffer PerFrame : register( b1 )
{
	float3 EyePos;		// the current camera position
	float m_emptyOne;	// Directx 11 requires constant buffer data to be a multiple of 16, so this is spare
}

// DO NOT CHANGE THIS STRUCT!!! The vertex format is being built at run time based on this struct
struct VIn
{
	float4 position : POSITION; // DO NOT CHANGE THIS STRUCT!!! The vertex format is being built at run time based on this struct
	float4 normal	: NORMAL; // DO NOT CHANGE THIS STRUCT!!! The vertex format is being built at run time based on this struct
	float4 texCoord	: TEXCOORD; // DO NOT CHANGE THIS STRUCT!!! The vertex format is being built at run time based on this struct
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 colour	: COLOR;
	float3 normal	: TEXCOORD0;
	float3 worldPos	: TEXCOORD1;
};

struct PIn
{
	float4 position : SV_POSITION;
	float4 colour	: COLOR;
	float3 normal	: TEXCOORD0;
	float3 worldPos	: TEXCOORD1;
};

VOut VShader( VIn input )
{
	VOut output;

	matrix wvp = mul( mul( World, View ), Projection );

	output.position = mul( input.position, wvp );

	// the pixel shader needs to know the world coordinate of the pixel for lighting
	output.worldPos = mul( input.position, World ).xyz;
	output.normal = input.normal.xyz;

	//output.position = input.position;
	output.colour = MATERIAL_COLOUR;

	return output;
}

float4 PShader( PIn input ) : SV_TARGET
{
	// take the pixel normal from the input - ensure it is normalised after the interpolation
	float3 pixelNormal = normalize( input.normal );

	float3 surfaceColour = input.colour.xyz;

	float3 totalDiffuseRef = 0.0;
	float3 totalSpecularIntensity = 0.0;

	// ambient light
	float3 ambientRef = AMBIENT_LIGHT_COLOUR * AMBIENT_COEFF;

	float3 lightVector = LIGHT_POS - input.worldPos;
	float distanceToLight = length( lightVector );
	lightVector = normalize( lightVector );

	float pixelDotLight = dot( pixelNormal, lightVector );

	float3 diffuseIntensity = DIFFUSE_LIGHT_COLOUR;
	float3 specularIntensity = SPECULAR_LIGHT_COLOUR;

	// if the dot product is less than 0, then the angle between the light and the surface normal is greater than 90 
	// degress which means the light is hitting the back of the surface
	if ( pixelDotLight > 0 )
	{
		// diffuse
		totalDiffuseRef += ( DIFFUSE_COEFF * pixelDotLight * diffuseIntensity );

		// specular
		float scaleConstant = 2.0 * pixelDotLight;

		float3 surfaceToEye = normalize( float3( EyePos - input.worldPos ) );

		float3 refRay = normalize( ( scaleConstant * pixelNormal ) - lightVector );

		totalSpecularIntensity += ( SPECULAR_COEFF * specularIntensity * pow( saturate( dot( surfaceToEye, refRay ) ), SHINY_COEFF ) );
	}

	//total surface colour = current colour * (ambient + diffuse) + specular	
	return float4( surfaceColour * ( ambientRef + totalDiffuseRef ) + totalSpecularIntensity, 1 );
}

*/