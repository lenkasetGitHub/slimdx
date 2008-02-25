/*
* Copyright (c) 2007-2008 SlimDX Group
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
#include <d3d9.h>
#include <d3dx9.h>
#include <vcclr.h>

#include "../ComObject.h"
#include "../Math/Matrix.h"
#include "../DataStream.h"
#include "../Utilities.h"

#include "Enums.h"
#include "Device.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "Texture.h"
#include "EffectStateManager.h"

using namespace System;

namespace SlimDX
{
namespace Direct3D9
{
	IEffectStateManagerShim::IEffectStateManagerShim( IEffectStateManager^ wrappedInterface )
	{
		m_WrappedInterface = wrappedInterface;
	}

	HRESULT IEffectStateManagerShim::LightEnable( DWORD Index, BOOL Enable )
	{
		try
		{
			m_WrappedInterface->EnableLight( Index, Enable > 0 );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT IEffectStateManagerShim::SetFVF( DWORD FVF )
	{
		try
		{
			m_WrappedInterface->SetFVF( static_cast<VertexFormat>( FVF ) );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT IEffectStateManagerShim::SetLight( DWORD index, const D3DLIGHT9* pLight )
	{
		try
		{
			Light light;
			light.Ambient = Color4::FromUnmanaged( pLight->Ambient );
			light.Diffuse = Color4::FromUnmanaged( pLight->Diffuse );
			light.Specular = Color4::FromUnmanaged( pLight->Specular );
			light.Attenuation0 = pLight->Attenuation0;
			light.Attenuation1 = pLight->Attenuation1;
			light.Attenuation2 = pLight->Attenuation2;
			light.Direction = Vector3( pLight->Direction.x, pLight->Direction.y, pLight->Direction.z );
			light.Position = Vector3( pLight->Position.x, pLight->Position.y, pLight->Position.z );
			light.Falloff = pLight->Falloff;
			light.Phi = pLight->Phi;
			light.Range = pLight->Range;
			light.Theta = pLight->Theta;
			light.Type = static_cast<LightType>( pLight->Type );

			m_WrappedInterface->SetLight( index, light );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT IEffectStateManagerShim::SetMaterial( const D3DMATERIAL9* pMaterial )
	{
		try
		{
			Material material;
			material.Ambient = Color4::FromUnmanaged( pMaterial->Ambient );
			material.Specular = Color4::FromUnmanaged( pMaterial->Specular );
			material.Diffuse = Color4::FromUnmanaged( pMaterial->Diffuse );
			material.Emissive = Color4::FromUnmanaged( pMaterial->Emissive );
			material.Power = pMaterial->Power;

			m_WrappedInterface->SetMaterial( material );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT IEffectStateManagerShim::SetNPatchMode( FLOAT nSegments )
	{
		try
		{
			m_WrappedInterface->SetNPatchMode( nSegments );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT IEffectStateManagerShim::SetTransform( D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix )
	{
		try
		{
			m_WrappedInterface->SetTransform( static_cast<TransformState>( State ), Matrix::FromD3DXMATRIX( *pMatrix ) );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT IEffectStateManagerShim::SetPixelShader( LPDIRECT3DPIXELSHADER9 pShader )
	{
		try
		{
			m_WrappedInterface->SetPixelShader( gcnew PixelShader( pShader ) );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT IEffectStateManagerShim::SetPixelShaderConstantB( UINT StartRegister, const BOOL* pConstantData, UINT RegisterCount )
	{
		try
		{
			array<bool>^ data = gcnew array<bool>( RegisterCount );
			for( UINT i = 0; i < RegisterCount; i++ )
				data[i] = (pConstantData[i] > 0);

			m_WrappedInterface->SetPixelShaderConstant( StartRegister, data );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT IEffectStateManagerShim::SetPixelShaderConstantF( UINT StartRegister, const FLOAT* pConstantData, UINT RegisterCount )
	{
		try
		{
			array<float>^ data = gcnew array<float>( RegisterCount );
			for( UINT i = 0; i < RegisterCount; i++ )
				data[i] = (pConstantData[i] > 0);

			m_WrappedInterface->SetPixelShaderConstant( StartRegister, data );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT IEffectStateManagerShim::SetPixelShaderConstantI( UINT StartRegister, const INT* pConstantData, UINT RegisterCount )
	{
		try
		{
			array<int>^ data = gcnew array<int>( RegisterCount );
			for( UINT i = 0; i < RegisterCount; i++ )
				data[i] = (pConstantData[i] > 0);

			m_WrappedInterface->SetPixelShaderConstant( StartRegister, data );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT IEffectStateManagerShim::SetVertexShader( LPDIRECT3DVERTEXSHADER9 pShader )
	{
		try
		{
			m_WrappedInterface->SetVertexShader( gcnew VertexShader( pShader ) );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT IEffectStateManagerShim::SetVertexShaderConstantB( UINT StartRegister, const BOOL* pConstantData, UINT RegisterCount )
	{
		try
		{
			array<bool>^ data = gcnew array<bool>( RegisterCount );
			for( UINT i = 0; i < RegisterCount; i++ )
				data[i] = (pConstantData[i] > 0);

			m_WrappedInterface->SetVertexShaderConstant( StartRegister, data );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT IEffectStateManagerShim::SetVertexShaderConstantF( UINT StartRegister, const FLOAT* pConstantData, UINT RegisterCount )
	{
		try
		{
			array<float>^ data = gcnew array<float>( RegisterCount );
			for( UINT i = 0; i < RegisterCount; i++ )
				data[i] = (pConstantData[i] > 0);

			m_WrappedInterface->SetVertexShaderConstant( StartRegister, data );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT IEffectStateManagerShim::SetVertexShaderConstantI( UINT StartRegister, const INT* pConstantData, UINT RegisterCount )
	{
		try
		{
			array<int>^ data = gcnew array<int>( RegisterCount );
			for( UINT i = 0; i < RegisterCount; i++ )
				data[i] = (pConstantData[i] > 0);

			m_WrappedInterface->SetVertexShaderConstant( StartRegister, data );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT IEffectStateManagerShim::SetRenderState( D3DRENDERSTATETYPE State, DWORD Value )
	{
		try
		{
			m_WrappedInterface->SetRenderState( static_cast<RenderState>( State ), Value );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT IEffectStateManagerShim::SetSamplerState( DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value )
	{
		try
		{
			m_WrappedInterface->SetSamplerState( Sampler, static_cast<SamplerState>( Type ), Value );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT IEffectStateManagerShim::SetTexture( DWORD Stage, LPDIRECT3DBASETEXTURE9 pTexture )
	{
		try
		{
			BaseTexture^ texture;

			switch( pTexture->GetType() )
			{
			case D3DRTYPE_TEXTURE:
				texture = gcnew Texture( static_cast<IDirect3DTexture9*>( pTexture ) );
			case D3DRTYPE_VOLUMETEXTURE:
				texture = gcnew VolumeTexture( static_cast<IDirect3DVolumeTexture9*>( pTexture ) );
			case D3DRTYPE_CUBETEXTURE:
				texture = gcnew CubeTexture( static_cast<IDirect3DCubeTexture9*>( pTexture ) );

			default:
				texture = nullptr;
			}

			m_WrappedInterface->SetTexture( Stage, texture );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT IEffectStateManagerShim::SetTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value )
	{
		try
		{
			m_WrappedInterface->SetTextureStageState( Stage, static_cast<TextureStage>( Type ), Value );
		}
		catch( Exception^ )
		{
			return E_FAIL;
		}

		return S_OK;
	}
}
}