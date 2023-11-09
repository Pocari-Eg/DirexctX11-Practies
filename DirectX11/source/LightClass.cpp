#include "LightClass.h"

LightClass::LightClass()
{
}

LightClass::LightClass(const LightClass&)
{
}

LightClass::~LightClass()
{
}

void LightClass::SetDiffuseColor(float r, float g, float b, float a)
{
    m_diffuseColor = XMFLOAT4(r, g, b, a);
}

void LightClass::SetDirection(float x, float y, float z)
{
    m_direction = XMFLOAT3(x, y, z);
}

void LightClass::SetAmbientColor(float r, float g, float b, float a)
{
    m_ambientColor = XMFLOAT4(r, g, b, a);
}

void LightClass::SetSpecularColor(float r, float g, float b, float a)
{
    m_specularColor = XMFLOAT4(r, g, b, a);
}

void LightClass::SetSpecularPower(float power)
{
    m_specularPower = power;
}

XMFLOAT4 LightClass::GetAmbientColor()
{
    return m_ambientColor;
}

XMFLOAT4 LightClass::GetDffiuseColor()
{
    return m_diffuseColor;
}

XMFLOAT3 LightClass::GetDirection()
{
    return m_direction;
}

XMFLOAT4 LightClass::GetSpecularColor()
{
    return m_specularColor;
}

float LightClass::GetSpecularPower()
{
    return m_specularPower;
}
