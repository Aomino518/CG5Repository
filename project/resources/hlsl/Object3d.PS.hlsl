#include "Object3d.hlsli"

struct Material
{
    float4 color;
    int enableLightig;
    float4x4 uvTransform;
    float shininess;
};

struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
};

struct PointLight
{
    float4 color;
    float3 position;
    float intensity;
    float radius;
    float decay;
};

struct SpotLight
{
    float4 color;
    float3 position;
    float intensity;
    float3 direction;
    float distance;
    float decay;
    float cosAngle;
    float cosFalloffStart;
};

struct Camera
{
    float3 worldPosition;
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<DirectionalLight> gDiretionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<PointLight> gPointLight : register(b3);
ConstantBuffer<SpotLight> gSpotLight : register(b4);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

float3 CalcDirectionalLight(VertexShaderOutput input, float specularPow, float4 textureColor)
{
    float NdotL = dot(normalize(input.normal), -gDiretionalLight.direction);
    float diffuseCos = saturate(NdotL);
    float3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDiretionalLight.color.rgb * diffuseCos * gDiretionalLight.intensity;
    float3 specular = gDiretionalLight.color.rgb * gDiretionalLight.intensity * specularPow;
        
    return diffuse + specular;
}

float3 CalcPointLight(VertexShaderOutput input, float specularPow, float4 textureColor, float3 lightDirPoint)
{
    float distance = length(gPointLight.position - input.worldPosition);
    float factor = pow(saturate(-distance / gPointLight.radius + 1.0f), gPointLight.decay);
    float NdotL = dot(normalize(input.normal), lightDirPoint);
    float diffuseCos = saturate(NdotL);
    float3 diffuse = gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * diffuseCos * gPointLight.intensity * factor;
    float3 specular = gPointLight.color.rgb * gPointLight.intensity * specularPow * factor;
    
    return diffuse + specular;
}

float3 CalcSpotLight(VertexShaderOutput input, float specularPow, float4 textureColor, float3 spotLightDirOnSurface)
{
    float distance = length(gSpotLight.position - input.worldPosition);
    float attenuationFactor = pow(saturate(-distance / gSpotLight.distance + 1.0f), gSpotLight.decay);
    float cosAngle = dot(normalize(-spotLightDirOnSurface), gSpotLight.direction);
    float falloffFactor = saturate((cosAngle - gSpotLight.cosAngle) / (gSpotLight.cosFalloffStart - gSpotLight.cosAngle));
    float NdotL = dot(normalize(input.normal), normalize(-spotLightDirOnSurface));
    float diffuseCos = saturate(NdotL);
    float3 diffuse = gMaterial.color.rgb * textureColor.rgb * gSpotLight.color.rgb * diffuseCos * gSpotLight.intensity * attenuationFactor * falloffFactor;
    float3 specular = gSpotLight.color.rgb * gSpotLight.intensity * specularPow * attenuationFactor * falloffFactor;
    
    return diffuse + specular;
}

PixelShaderOutput main(VertexShaderOutput input)
{
    // 視線ベクトル
    float3 toEyeDirection = normalize(gCamera.worldPosition - input.worldPosition);
    float3 halfVectorDirection = normalize(-gDiretionalLight.direction + toEyeDirection);
    float NdotHDir = dot(normalize(input.normal), halfVectorDirection);
    float specularPowDir = pow(saturate(NdotHDir), gMaterial.shininess);
    
    float3 toEyePoint = normalize(gCamera.worldPosition - input.worldPosition);
    float3 lightDirPoint = normalize(gPointLight.position - input.worldPosition);
    float3 halfVectorPoint = normalize(lightDirPoint + toEyePoint);
    float NdotHPoint = dot(normalize(input.normal), halfVectorPoint);
    float specularPowPoint = pow(saturate(NdotHPoint), gMaterial.shininess);
    
    float3 toEyeSpot = normalize(gCamera.worldPosition - input.worldPosition);
    float3 lightDirSpot = normalize(gSpotLight.position - input.worldPosition);
    float3 halfVectorSpot = normalize(lightDirSpot + toEyeSpot);
    float NdotHSpot = dot(normalize(input.normal), halfVectorSpot);
    float specularPowSpot = pow(saturate(NdotHSpot), gMaterial.shininess);
    
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    float3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
    
    float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gSpotLight.position);
 
    PixelShaderOutput output;
    if (gMaterial.enableLightig != 0)
    {
        output.color.rgb = CalcDirectionalLight(input, specularPowDir, textureColor) + 
        CalcPointLight(input, specularPowPoint, textureColor, lightDirPoint) + 
        CalcSpotLight(input, specularPowPoint, textureColor, spotLightDirectionOnSurface);

        if (textureColor.a == 0.0f)
        {
            discard;
        }
        
        output.color.a = gMaterial.color.a * textureColor.a;
        
        if (output.color.a == 0.0f)
        {
            discard;
        }
    }
    else
    {
        output.color = gMaterial.color * textureColor;
        
        if (output.color.a == 0.0f)
        {
            discard;
        }
    }
    return output;
}