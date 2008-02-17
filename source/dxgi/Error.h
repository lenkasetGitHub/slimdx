/*
* Copyright (c) 2007 SlimDX Group
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
#pragma once

using namespace System;

#include "../Result.h"

namespace SlimDX
{
	namespace DXGI
	{
		public ref class Error sealed
		{
			private:
				Error();
				
			public:
				property static Result InvalidCall
				{
					Result get();
				}
				
				property static Result NotFound
				{
					Result get();
				}
				
				property static Result MoreData
				{
					Result get();
				}
				
				property static Result Unsupported
				{
					Result get();
				}
				
				property static Result DeviceRemoved
				{
					Result get();
				}
				
				property static Result DeviceHung
				{
					Result get();
				}
				
				property static Result DeviceReset
				{
					Result get();
				}
				
				property static Result WasStillDrawing
				{
					Result get();
				}
				
				property static Result DriverInternalError
				{
					Result get();
				}
				
				property static Result NonExclusive
				{
					Result get();
				}
		};
	}
}