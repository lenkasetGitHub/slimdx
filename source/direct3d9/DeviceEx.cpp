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
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>

#include "../ComObject.h"

#include "Direct3D9Exception.h"

#include "DeviceEx.h"
#include "PresentParameters.h"
#include "Resource.h"

using namespace System;

namespace SlimDX
{
namespace Direct3D9
{
	DeviceEx::DeviceEx( IDirect3DDevice9Ex* pointer )
		: Device( true )
	{
		Construct( pointer );
	}

	DeviceEx::DeviceEx( IntPtr pointer )
		: Device( true )
	{
		Construct( pointer, NativeInterface );
	}

	void DeviceEx::Internal_Constructor( Direct3DEx^ direct3D, int adapter, DeviceType deviceType, System::IntPtr controlHandle,
				CreateFlags createFlags, PresentParameters^ presentParameters, D3DDISPLAYMODEEX* fullscreenDisplayMode )
	{
		IDirect3DDevice9Ex* device;
		D3DPRESENT_PARAMETERS d3dpp = presentParameters->ToUnmanaged();

		HRESULT hr = direct3D->InternalPointer->CreateDeviceEx( adapter,
			static_cast<D3DDEVTYPE>( deviceType ),
			static_cast<HWND>( controlHandle.ToPointer() ), 
			static_cast<DWORD>( createFlags ),
			&d3dpp, fullscreenDisplayMode, &device );
		
		if( RECORD_D3D9( hr ).IsFailure )
			throw gcnew Direct3D9Exception( Result::Last );

		presentParameters->BackBufferCount = d3dpp.BackBufferCount;
		presentParameters->BackBufferFormat = static_cast<Format>( d3dpp.BackBufferFormat );
		presentParameters->BackBufferWidth = d3dpp.BackBufferWidth;
		presentParameters->BackBufferHeight = d3dpp.BackBufferHeight;

		Construct(device);
	}

	DeviceEx::DeviceEx( Direct3DEx^ direct3D, int adapter, DeviceType deviceType, System::IntPtr controlHandle,
		CreateFlags createFlags, PresentParameters^ presentParameters )
		: Device( true )
	{
		Internal_Constructor( direct3D, adapter, deviceType, controlHandle, createFlags, presentParameters, NULL );
	}

	DeviceEx::DeviceEx( Direct3DEx^ direct3D, int adapter, DeviceType deviceType, System::IntPtr controlHandle,
		CreateFlags createFlags, PresentParameters^ presentParameters, DisplayModeEx fullscreenDisplayMode )
		: Device( true )
	{
		D3DDISPLAYMODEEX nativeMode = fullscreenDisplayMode.ToUnmanaged();
		Internal_Constructor( direct3D, adapter, deviceType, controlHandle, createFlags, presentParameters, &nativeMode );
	}

	DeviceEx^ DeviceEx::FromPointer( IDirect3DDevice9Ex* pointer )
	{
		if( pointer == 0 )
			return nullptr;

		DeviceEx^ tableEntry = safe_cast<DeviceEx^>( ObjectTable::Find( static_cast<IntPtr>( pointer ) ) );
		if( tableEntry != nullptr )
		{
			pointer->Release();
			return tableEntry;
		}

		return gcnew DeviceEx( pointer );
	}

	DeviceEx^ DeviceEx::FromPointer( IntPtr pointer )
	{
		if( pointer == IntPtr::Zero )
			throw gcnew ArgumentNullException( "pointer" );

		DeviceEx^ tableEntry = safe_cast<DeviceEx^>( ObjectTable::Find( static_cast<IntPtr>( pointer ) ) );
		if( tableEntry != nullptr )
		{
			return tableEntry;
		}

		return gcnew DeviceEx( pointer );
	}

	int DeviceEx::GpuThreadPriority::get()
	{
		int priority;
		HRESULT hr = InternalPointer->GetGPUThreadPriority( &priority );
		RECORD_D3D9( hr );

		return priority;
	}

	void DeviceEx::GpuThreadPriority::set( int priority )
	{
		HRESULT hr = InternalPointer->SetGPUThreadPriority( priority );
		RECORD_D3D9( hr );
	}

	int DeviceEx::MaximumFrameLatency::get()
	{
		int maxLatency;
		HRESULT hr = InternalPointer->GetMaximumFrameLatency( reinterpret_cast<UINT*>( &maxLatency ) );
		RECORD_D3D9( hr );

		return maxLatency;
	}

	void DeviceEx::MaximumFrameLatency::set( int maxLatency )
	{
		HRESULT hr = InternalPointer->SetMaximumFrameLatency( maxLatency );
		RECORD_D3D9( hr );
	}

	DeviceState DeviceEx::CheckDeviceState( System::IntPtr windowHandle )
	{
		HRESULT hr = InternalPointer->CheckDeviceState( static_cast<HWND>( windowHandle.ToPointer() ) );
		return static_cast<DeviceState>( hr );
	}

	ResourceResidency DeviceEx::CheckResourceResidency( Resource^ resource )
	{
		IDirect3DResource9* resourcePointer = resource->InternalPointer;
		HRESULT hr = InternalPointer->CheckResourceResidency( &resourcePointer, 1 );
		return static_cast<ResourceResidency>( hr );
	}

	ResourceResidency DeviceEx::CheckResourceResidency( array<Resource^>^ resources )
	{
		std::vector<IDirect3DResource9*> resourceArray;
		resourceArray.reserve( resources->Length );

		for each( Resource^ resource in resources )
		{
			resourceArray.push_back( resource->InternalPointer );
		}

		HRESULT hr = InternalPointer->CheckResourceResidency( &resourceArray[0], resourceArray.size() );
		return static_cast<ResourceResidency>( hr );
	}

	DisplayModeEx DeviceEx::GetDisplayModeEx( int swapChain )
	{
		D3DDISPLAYMODEEX nativeMode = {0};
		HRESULT hr = InternalPointer->GetDisplayModeEx( swapChain, &nativeMode, NULL );
		RECORD_D3D9( hr );

		DisplayModeEx mode = DisplayModeEx::FromUnmanaged( nativeMode );
		return mode;
	}

	DisplayModeEx DeviceEx::GetDisplayModeEx( int swapChain, [Out] DisplayRotation% rotation )
	{
		D3DDISPLAYMODEEX nativeMode = {0};
		D3DDISPLAYROTATION nativeRotation = D3DDISPLAYROTATION_IDENTITY;

		HRESULT hr = InternalPointer->GetDisplayModeEx( swapChain, &nativeMode, &nativeRotation );
		RECORD_D3D9( hr );

		rotation = static_cast<DisplayRotation>( nativeRotation );
		DisplayModeEx mode = DisplayModeEx::FromUnmanaged( nativeMode );
		return mode;
	}

	Result DeviceEx::PresentEx( SlimDX::Direct3D9::Present flags )
	{
		HRESULT hr = InternalPointer->PresentEx( 0, 0, 0, 0, static_cast<DWORD>( flags ) );
		RECORD_D3D9( hr );

		return Result::Last;
	}

	Result DeviceEx::PresentEx( System::IntPtr windowOverride, SlimDX::Direct3D9::Present flags )
	{
		HRESULT hr = InternalPointer->PresentEx( 0, 0, static_cast<HWND>( windowOverride.ToPointer() ), 0, static_cast<DWORD>( flags ) );
		RECORD_D3D9( hr );

		return Result::Last;
	}
	
	Result DeviceEx::ResetEx( PresentParameters^ presentParameters )
	{
		D3DPRESENT_PARAMETERS d3dpp;

		d3dpp = presentParameters->ToUnmanaged();
		HRESULT hr = InternalPointer->ResetEx( &d3dpp, NULL );
		RECORD_D3D9( hr );

		presentParameters->BackBufferCount = d3dpp.BackBufferCount;
		presentParameters->BackBufferFormat = static_cast<Format>( d3dpp.BackBufferFormat );
		presentParameters->BackBufferWidth = d3dpp.BackBufferWidth;
		presentParameters->BackBufferHeight = d3dpp.BackBufferHeight;

		return Result::Last;
	}

	Result DeviceEx::ResetEx( PresentParameters^ presentParameters, DisplayModeEx fullscreenDisplayMode )
	{
		D3DPRESENT_PARAMETERS d3dpp = presentParameters->ToUnmanaged();
		D3DDISPLAYMODEEX nativeDisplayMode = fullscreenDisplayMode.ToUnmanaged();
		HRESULT hr = InternalPointer->ResetEx( &d3dpp, &nativeDisplayMode );
		RECORD_D3D9( hr );

		presentParameters->BackBufferCount = d3dpp.BackBufferCount;
		presentParameters->BackBufferFormat = static_cast<Format>( d3dpp.BackBufferFormat );
		presentParameters->BackBufferWidth = d3dpp.BackBufferWidth;
		presentParameters->BackBufferHeight = d3dpp.BackBufferHeight;

		return Result::Last;
	}

	void DeviceEx::WaitForVBlank( int swapChain )
	{
		//This method will always return D3D_OK.
		InternalPointer->WaitForVBlank( swapChain );
	}
}
}