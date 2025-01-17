/*
* Copyright (c) 2007-2017, Intel Corporation
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/
//!
//! \file      cm_device.h
//! \brief     Contains CmDevice declarations.
//!

#ifndef MEDIADRIVER_LINUX_COMMON_CM_CMDEVICE_H_
#define MEDIADRIVER_LINUX_COMMON_CM_CMDEVICE_H_

#include "cm_def.h"
#include "mos_os.h"

namespace CMRT_UMD
{
class CmQueue;
class CmKernel;
class CmThreadSpace;
class CmThreadGroupSpace;
class CmVebox;
class CmSampler;
class CmEvent;
class CmTask;
class CmProgram;
class CmBuffer;
class CmBufferUP;
class CmBufferSVM;
class CmSurface2D;
class CmSurface2DUP;
class CmSurface2DUPRT;
class CmSurface3D;
class CmSampler8x8;

//! \brief  CmDevice class \@UMD for Linux
class CmDevice
{
public:
    //!
    //! \brief      Creates a CmBuffer with specified size in bytes.
    //! \details    This function creates a buffer in video memory with linear
    //!             layout.
    //! \param      [in] size
    //!             Buffer size in bytes.
    //! \param      [out] surface
    //!             Reference to the pointer to the CmBuffer.
    //! \retval     CM_SUCCESS if the CmBuffer is successfully created.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of system memory.
    //! \retval     CM_SURFACE_ALLOCATION_FAILURE if creating the underneath 1D
    //!             surface fails.
    //! \retval     CM_INVALID_WIDTH if width is less than CM_MIN_SURF_WIDTH or
    //!             larger than CM_MAX_1D_SURF_WIDTH.
    //! \retval     CM_EXCEED_SURFACE_AMOUNT if maximum amount of 1D surfaces
    //!             is exceeded. The amount is the amount of the surfaces
    //!             that can co-exist. The amount can be obtained by querying
    //!             the cap CAP_BUFFER_COUNT.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t CreateBuffer(uint32_t size,
                                           CmBuffer* &surface) = 0;

    //!
    //! \brief    Creates a CmBuffer from an existing MOS Resource.
    //! \details  CmBuffer is a wrapper of that MOS resource. This Mos resource is
    //!            owned by caller.
    //! \param    [in] mosResource
    //!           pointer to MOS resource.
    //! \param    [in,out] surface
    //!           reference to pointer of surface to be created.
    //! \retval   CM_SUCCESS if the CmBuffer is successfully created.
    //! \retval   CM_INVALID_MOS_RESOURCE_HANDLE if mosResource is nullptr.
    //! \retval   CM_OUT_OF_HOST_MEMORY if out of system memory
    //! \retval   CM_EXCEED_SURFACE_AMOUNT if maximum amount of 1D surfaces is exceeded.
    //! \retval   CM_FAILURE otherwise
    //!
    CM_RT_API virtual int32_t CreateBuffer(PMOS_RESOURCE mosResource,
                                           CmBuffer* &surface) = 0;

    //!
    //! \brief      Creates a CmSurface2D with given width, height, and format.
    //! \details    This function creates a surface in video memory with a 2D layout.
    //!             User needs to provide width, height and format.
    //! \param      [in] width
    //!             Surface width in pixel.
    //! \param      [in] height
    //!             Surface height in pixel.
    //! \param      [in] format
    //!             Surface format.
    //! \param      [out] surface
    //!             Reference to the pointer to the CmSurface2D.
    //! \retval     CM_SUCCESS if the CmSurface2D is successfully created.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of system memory.
    //! \retval     CM_SURFACE_ALLOCATION_FAILURE if creating the underneath
    //!             resource fails.
    //! \retval     CM_INVALID_WIDTH if width is less than CM_MIN_SURF_WIDTH or
    //!             larger than CM_MAX_2D_SURF_WIDTH, or for YUY2, or NV12
    //!             format, the width is odd.
    //! \retval     CM_INVALID_HEIGHT if height is less than CM_MIN_SURF_HEIGHT
    //!             or larger than CM_MAX_2D_SURF_HEIGHT, or for NV12 format,
    //!             the height is odd.
    //! \retval     CM_SURFACE_FORMAT_NOT_SUPPORTED if the format is not
    //!             supported. The supported formats can be obtained by
    //!             querying cap CAP_SURFACE2D_FORMAT_COUNT and
    //!             CAP_SURFACE2D_FORMATS. For now, the following formats are
    //!             supported: \n
    //!             CM_SURFACE_FORMAT_A8R8G8B8, \n
    //!             CM_SURFACE_FORMAT_X8R8G8B8, \n
    //!             CM_SURFACE_FORMAT_A8B8G8R8, \n
    //!             CM_SURFACE_FORMAT_R32F, \n
    //!             CM_SURFACE_FORMAT_V8U8, \n
    //!             CM_SURFACE_FORMAT_P8, \n
    //!             CM_SURFACE_FORMAT_YUY2, \n
    //!             CM_SURFACE_FORMAT_A8, \n
    //!             CM_SURFACE_FORMAT_NV12, \n
    //!             CM_SURFACE_FORMAT_P010, \n
    //!             CM_SURFACE_FORMAT_UYVY, \n
    //!             CM_SURFACE_FORMAT_IMC3, \n
    //!             CM_SURFACE_FORMAT_411P, \n
    //!             CM_SURFACE_FORMAT_422H, \n
    //!             CM_SURFACE_FORMAT_422V, \n
    //!             CM_SURFACE_FORMAT_444P, \n
    //!             CM_SURFACE_FORMAT_YV12, \n
    //!             CM_SURFACE_FORMAT_R8_UINT, \n
    //!             CM_SURFACE_FORMAT_R16_UINT. \n
    //! \retval     CM_EXCEED_SURFACE_AMOUNT if maximum amount of 2D surfaces
    //!             is exceeded. The amount is the amount of the surfaces that
    //!             can co-exist. The amount can be obtained by querying the
    //!             cap CAP_SURFACE2D_COUNT.
    //! \retval     CM_FAILURE otherwise.
    //! \note       For planar surface, there is only one CmSurface2D instance,
    //!             no matter how many planes the surface may have.
    //!             The detail about how to access different planes in the kernel
    //!             code can be found in CM Language Spec, looking for read_plane
    //!             and write_plane.
    //!
    CM_RT_API virtual int32_t CreateSurface2D(uint32_t width,
                                              uint32_t height,
                                              CM_SURFACE_FORMAT format,
                                              CmSurface2D* &surface) = 0;

    //!
    //! \brief    Creates a CmSurface2D from an existing MOS Resource.
    //! \details  CmSurface2D is a wrapper of that MOS resource. This Mos resource is
    //!            owned by caller.
    //! \param    [in] mosResource
    //!           pointer to MOS resource.
    //! \param    [in,out] surface
    //!           reference to pointer of surface to be created.
    //! \retval   CM_SUCCESS if the CmSurface2D is successfully created.
    //! \retval   CM_INVALID_MOS_RESOURCE_HANDLE if pMosResrouce is nullptr.
    //! \retval   CM_EXCEED_SURFACE_AMOUNT if maximum amount of 2D surfaces
    //!           is exceeded.
    //! \retval   CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t CreateSurface2D(PMOS_RESOURCE mosResource,
                                              CmSurface2D* &surface) = 0;

    //!
    //! \brief    Creates a CmSurface2D from an existing VA surface.
    //! \details  The application must have created the VA surface using the
    //!           VADriverContext used to create CmDevice. The VA surface format
    //!           should be within the supported format set which can be obtained
    //!           by querying cap CAP_SURFACE2D_FORMAT_COUNT and CAP_SURFACE2D_FORMATS.
    //! \param    [in] vaSurface
    //!           indext to VA surface.
    //! \param    [in] vaDriverCtx
    //!           pointer to VA driver context.
    //! \param    [in,out] surface
    //!           reference to pointer of surface to be created.
    //! \retval   CM_SUCCESS if the CmSurface2D is successfully created.
    //! \retval   CM_EXCEED_SURFACE_AMOUNT if maximum amount of 2D surfaces
    //!           is exceeded.
    //! \retval   CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t CreateSurface2D(VASurfaceID vaSurface,
                                              VADriverContext *vaDriverCtx,
                                              CmSurface2D* &surface) = 0;

    //!
    //! \brief      Creates a CmSurface3D with given width, height, depth and
    //!             pixel format.
    //! \details    This function creates a surface in memory with a 3D layout.
    //!             User needs to provide width, height, depth and format.
    //! \param      [in] width
    //!             Surface width.
    //! \param      [in] height
    //!             Surface height.
    //! \param      [in] depth
    //!             Surface depth.
    //! \param      [in] format
    //!             Surface format.
    //! \param      [out] surface
    //!             Reference to the pointer to the CmSurface3D.
    //! \retval     CM_SUCCESS if the CmSurface3D is successfully created.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of system memory.
    //! \retval     CM_INVALID_WIDTH if width is less than CM_MIN_SURF_WIDTH or
    //!             larger than CM_MAX_3D_SURF_WIDTH.
    //! \retval     CM_INVALID_HEIGHT if height is less than CM_MIN_SURF_HEIGHT
    //!             or larger than CM_MAX_3D_SURF_HEIGHT.
    //! \retval     CM_INVALID_DEPTH if width is less than CM_MIN_SURF_DEPTH or
    //!             larger than CM_MAX_3D_SURF_DEPTH.
    //! \retval     CM_SURFACE_FORMAT_NOT_SUPPORTED if the format is not
    //!             supported. The supported formats can be obtained by
    //!             querying cap CAP_SURFACE3D_FORMAT_COUNT and
    //!             CAP_SURFACE3D_FORMATS, For now, only supports: \n
    //!             CM_SURFACE_FORMAT_X8R8G8B8, \n
    //!             CM_SURFACE_FORMAT_A8R8G8B8, \n
    //!             CM_SURFACE_FORMAT_A16B16G16R16. \n
    //! \retval     CM_SURFACE_ALLOCATION_FAILURE if creating the underneath 3D
    //!             surface fails.
    //! \retval     CM_EXCEED_SURFACE_AMOUNT if maximum amount of 3D surfaces
    //!             is exceeded. The amount is the amount of the surfaces that
    //!             can co-exist. The amount can be obtained by querying the
    //!             cap CAP_SURFACE3D_COUNT.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t CreateSurface3D(uint32_t width,
                                              uint32_t height,
                                              uint32_t depth,
                                              CM_SURFACE_FORMAT format,
                                              CmSurface3D* &surface) = 0;

    //!
    //! \brief      Destroys CmBuffer object.
    //! \details    This function destroys CmBuffer object. After the function
    //!             is called, it will return immediately without waiting. If
    //!             there is any Enqueue is being executed when this function
    //!             is called, the actual destroy will be postponed internally
    //!             by the runtime, and user doens't need to worry about it.
    //! \param      [in,out] surface
    //!             Reference to the pointer pointing to CmBuffer, it will be
    //!             assigned to nullptr after destroy.
    //! \retval     CM_SUCCESS if CmBuffer is successfully destroyed.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t DestroySurface(CmBuffer* &surface) = 0;

    //!
    //! \brief      Destroys CmSurface2D type surface.
    //! \details    This function destroys CmSurface2D object. After the function
    //!             is called, it will return immediately without waiting. If
    //!             there is any Enqueue is being executed when this function is
    //!             called, the actual destroy will be postponed internally by
    //!             the runtime, and user doens't need to worry about it. One
    //!             exception is that if the CmSurface2D was created by a third
    //!             party VA surface, user has to keep the VA surface until the
    //!             kernel using it finishes execution.
    //! \param      [in,out] surface
    //!             Reference to the pointer pointing to CmSurface2D. It will
    //!             be assigned to nullptr after destroy.
    //! \retval     CM_SUCCESS if CmSurface2D is successfully destroyed.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t DestroySurface(CmSurface2D* &surface) = 0;

    //!
    //! \brief      Destroys CmSurface3D object.
    //! \details    This function destroys CmSurface3D object. After the
    //!             function is called, it will return immediately without
    //!             waiting. If there is any Enqueue is being executed when this
    //!             function is called, the actual destroy will be postponed
    //!             internally by the runtime, and user doens't need to worry
    //!             about it.
    //! \param      [in,out] surface
    //!             Reference to the pointer pointing to CmSurface3D. It will
    //!             be assigned to nullptr after destroy.
    //! \retval     CM_SUCCESS if CmSurface3D is successfully destroyed.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t DestroySurface(CmSurface3D* &surface) = 0;

    //!
    //! \brief      Creates a task queue.
    //! \details    CmQueue is an in-order queue of tasks. Each task is
    //!             essentially a CmTask object containing kernels that are to
    //!             be run concurrently. Each kernel can be executed with
    //!             multiple threads. Only one CmQueue is supported per CmDevice for now.
    //!             Trying to create a second CmQueue will return a previously
    //!             created CmQueue object.
    //! \param      [in,out] queue
    //!             Reference to the pointer to the CmQueue.
    //! \retval     CM_SUCCESS if the CmQueue is successfully created.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of system memory.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t CreateQueue(CmQueue* &queue) = 0;

    //!
    //! \brief      Creates a CmProgram object consisting of kernels loaded from
    //!             the commonIsaCode code.
    //! \details    Common ISA code is offline generated as a file with a .isa suffix
    //!             by the CM compiler when it is used to compile one or more
    //!             kernels. It contains ISA code common for all Intel platforms.
    //!             Just-In-Time (JIT) compilation of the common ISA code happens in
    //!             LoadProgram and generates platform specific ISA according to the
    //!             actaully platform where the application in running in hardware mode.
    //!             In the emulation mode JIT doesn't happen.
    //!             In the simulation mode JIT doesn't happen but paltform specfic
    //!             ISA need to be offline generated together with common ISA by
    //!             CM compiler and to be included in commonIsaCode.
    //!             How to generate common ISA and platform specific ISA can be
    //!             found in CM compiler manual.
    //! \param      [in] commonIsaCode
    //!             Pointer pointing to code in common ISA.
    //! \param      [in] size
    //!             Size in bytes of the common ISA code.
    //! \param      [in,out] program
    //!             Reference to the pointer to the CmProgram.
    //! \param      [in] options
    //!             JIT options for all kernels in the code. This argument
    //!             is optional. Size of options should be no more than 512
    //!             (CM_MAX_OPTION_SIZE_IN_BYTE) bytes including the null
    //!             terminator. There is one option available currently: \n
    //!             "nojitter" -- Use this option to completely disable jitter
    //!             from occurring. NOTE: "/Qxcm_jit_tartget=%GEN_ARCH%"
    //!             flag must be set during offline compilation if "nojitter"
    //!             is set in hardware mode.
    //!             In simulation and emulation mode, this option is ignored.
    //! \retval     CM_SUCCESS if the CmProgram is successfully created.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of system memory.
    //! \retval     CM_INVALID_ARG_VALUE if invalid input parameters.
    //! \retval     CM_INVALID_GENX_BINARY if the GEN binary is not matched with
    //!             actual running platform in "nojitter" mode.
    //! \retval     CM_JIT_COMPILE_FAILURE if JIT compile of the common ISA
    //!             code fails.
    //! \retval     CM_INVALID_KERNEL_SPILL_CODE if kernel has spill code and
    //!             devcie's scratch memory space is disabled in
    //!             CreateCmDeviceEx.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t LoadProgram(void *commonIsaCode,
                                          const uint32_t size,
                                          CmProgram* &program,
                                          const char *options = nullptr) = 0;

    //!
    //! \brief      Creates a CmKernel object from the CmProgram object.
    //! \details    A Cmprogram can contains multiple kernels.
    //!             The size of all arguments of a kernel should be no more than
    //!             CAP_ARG_SIZE_PER_KERNEL byte. The number of all kernel
    //!             arguments should be no more than CAP_ARG_COUNT_PER_KERNEL.
    //!             The size of kernel binary should be no more than
    //!             CAP_KERNEL_BINARY_SIZE bytes. The kernelName should be no
    //!             more than 256 (CM_MAX_KERNEL_NAME_SIZE_IN_BYTE) bytes
    //!             including the null terminator.
    //! \param      [in] program
    //!             CmProgram object from which the kernel is created.
    //! \param      [in] kernelName
    //!             CM kernel function (genx_main) name.  A CM_KERNEL_FUNCTION
    //!             macro MUST be used to specify this argument.
    //! \param      [in,out] kernel
    //!             Reference to the pointer to the CmKernel object.
    //! \param      [in] options
    //!             JIT options for this specific kernel, overwriting the JIT
    //!             options specified for all kernels in the CmProgram. This
    //!             argument is optional. Size of options should be no more
    //!             than 512 (CM_MAX_OPTION_SIZE_IN_BYTE) bytes including the
    //!             null terminator. No options available for now.
    //! \retval     CM_SUCCESS if the CmKernel is successfully created or
    //!             returned.
    //! \retval     CM_INVALID_ARG_VALUE if the program is an invalid pointer.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of system memory.
    //! \retval     CM_EXCEED_KERNEL_ARG_AMOUNT if the argument number of the
    //!             kernel fucntion is larger than CAP_ARG_COUNT_PER_KERNEL.
    //! \retval     CM_EXCEED_KERNEL_ARG_SIZE_IN_BYTE if the argument size of
    //!             the kernel fucntion is larger than CAP_ARG_SIZE_PER_KERNEL.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t CreateKernel(CmProgram *program,
                                           const char *kernelName,
                                           CmKernel* &kernel,
                                           const char *options = nullptr) = 0;

    //!
    //! \brief      Creates a CmSampler object.
    //! \details    This function creates a 3D sampler state object used to sample
    //!             a 2D surface.
    //! \param      [in] sampleState
    //!             Const reference to a CM_SAMPLER_STATE specifying the
    //!             characteristics of the sampler to be created. The structure
    //!             is defined below.
    //! \param      [out] sampler
    //!             Reference to the pointer to the CmSampler object.
    //! \retval     CM_SUCCESS if the CmSampler is successfully created.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of system memory.
    //! \retval     CM_EXCEED_SAMPLER_AMOUNT if maximum amount of sampler is
    //!             exceeded. The amount is the amount of the sampler that can
    //!             co-exist. The amount can be obtained by querying the cap
    //!             CAP_SAMPLER_COUNT.
    //! \retval     CM_FAILURE otherwise.
    //! \note       typedef struct _CM_SAMPLER_STATE\n
    //!             {\n
    //!                 CM_TEXTURE_FILTER_TYPE      minFilterType;\n
    //!                 CM_TEXTURE_FILTER_TYPE      magFilterType;\n
    //!                 CM_TEXTURE_ADDRESS_TYPE     addressU;\n
    //!                 CM_TEXTURE_ADDRESS_TYPE     addressV;\n
    //!                 CM_TEXTURE_ADDRESS_TYPE     addressW;\n
    //!             } CM_SAMPLER_STATE;\n
    //!
    //!             For now, only linear and anisotropic filter types are
    //!             supported for hardware and simulation modes. For emulation
    //!             mode, linear filter type is supported. Wrap, mirror, and
    //!             clamp address types are supported in hardware and simulation modes.
    //!             only clamp address type is supported in emulation mode.
    //!
    CM_RT_API virtual int32_t
    CreateSampler(const CM_SAMPLER_STATE &sampleState,
                  CmSampler* &sampler) = 0;

    //!
    //! \brief      Destroys a CmKernel.
    //! \details    A CmKernel that is not destroyed by calling this function
    //!             will be destroyed when the CmDevice is destroyed.
    //! \param      [in,out] kernel
    //!             CmKernel object to be destroyed. It will be assigned to
    //!             nullptr once the fuction is return.
    //! \retval     CM_SUCCESS if the CmKernel is successfully destroyed.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t DestroyKernel(CmKernel* &kernel) = 0;

    //!
    //! \brief      Destroys a CmSampler.
    //! \details    A CmSampler that is not destroyed by calling this function
    //!             will be destroyed when the CmDevice is destroyed.
    //! \param      [in,out] sampler
    //!             A reference to the CmSampler pointer.
    //! \retval     CM_SUCCESS if the CmSampler is successfully destroyed.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t DestroySampler(CmSampler* &sampler) = 0;

    //!
    //! \brief      Destroys a CmProgram.
    //! \details    A CmProgram that is not destroyed by calling this function
    //!             will be destroyed when the CmDevice is destroyed.
    //! \param      [in,out] program
    //!             Reference to the pointer to the CmProgram. It will be assigned to
    //!             nullptr once the fuction is return.
    //! \retval     CM_SUCCESS if the CmProgram is successfully destroyed.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t DestroyProgram(CmProgram* &program) = 0;

    //!
    //! \brief      Destroys a CmThreadSpace instance.
    //! \details    A CmThreadSpace that is not destroyed by calling this
    //!             function will be destroyed when the CmDevice is destroyed.
    //! \param      [in,out] threadSpace
    //!             Reference to the pointer to the CmThreadSpace. It will be
    //!             assigned to nullptr once the fuction is return.
    //! \retval     CM_SUCCESS if the CmThreadSpace is successfully destroyed.
    //! \retval     CM_FAILURE if the input is nullptr or not valid.
    //!
    CM_RT_API virtual int32_t DestroyThreadSpace(CmThreadSpace* &threadSpace) = 0;

    //!
    //! \brief      Creates a CmTask object
    //! \details    This object is a container for one or multiple CmKernel objects, and used
    //!             to enqueue the kernels for concurrent execution.
    //! \param      [out] task Reference to the pointer to the CmTask
    //! \retval     CM_SUCCESS if the CmTask is successfully created
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of system memory
    //!
    CM_RT_API virtual int32_t CreateTask(CmTask* &task) = 0;

    //!
    //! \brief      Destroy a CmTask object
    //! \details    A CmTask that is not destroyed by calling this
    //!             function will be destroyed when the CmDevice is destroyed
    //! \param      [in, out] task Reference to the pointer to the CmTask.
    //! \retval     CM_SUCCESS if the CmTaskis successfully destroyed
    //! \retval     CM_FAILURE otherwise
    //!
    CM_RT_API virtual int32_t DestroyTask(CmTask* &task) = 0;

    //!
    //! \brief    This function can be used to get HW capability.
    //! \details  Since Emulation mode is run during kernel
    //!           development the values returned in Emulation mode reflect the
    //!           capabilities at that time, and might not reflect
    //!           the latest HW capabilities using a later driver release
    //! \param    [in] capName Name of cap to query
    //! \param    [out] capValueSize Reference to the size in bytes of the Cap
    //!           value. On entry application should set this to the size
    //!           of memory allocated for the cap value. Application should
    //!           make sure this size is large enough to hold the Cap value
    //!           requested. On return from this function the actual size of
    //!           cap value is returned
    //! \param    [out] capValue Pointer pointing to memory where the
    //!           cap value should be returned
    //! \retval   CM_SUCCESS if the input capValueSize equals or
    //!           is larger than required Cap size and Cap Value
    //!           is successfully returned,
    //! \retval   CM_FAILURE otherwise
    //! \details
    //!  <table>
    //!    <tr>
    //!     <th>Cap Name</th>
    //!     <th>Size in bytes  </th>
    //!     <th>Type of Value</th>
    //!     <th>Description</th>
    //!   </tr>
    //!   <tr>
    //!     <td>CAP_KERNEL_COUNT_PER_TASK</td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Maximum number of kernels that can be enqueued in one task</td>
    //!   </tr>
    //!   <tr>
    //!     <td> CAP_KERNEL_BINARY_SIZE</td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td> Maximum kernel binary size in bytes</td>
    //!   </tr>
    //!   <tr>
    //!     <td> CAP_SAMPLER_COUNT</td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td> Maximum number of samplers that can co-exist at any time
    //!          in a CmDevice</td>
    //!   </tr>
    //!   <tr>
    //!     <td>CAP_SAMPLER_COUNT_PER_KERNEL </td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Maximum number of samplers that one kernel can use</td>
    //!   </tr>
    //!   <tr>
    //!     <td>CAP_BUFFER_COUNT </td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Maximum number of CmBuffer that can co - exist at any time </td>
    //!   </tr>
    //!     <tr>
    //!     <td> CAP_SURFACE2D_COUNT</td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Maximum number of CmSurface2D that can co-exist at
    //!             any time </td>
    //!   </tr>
    //!   <tr>
    //!     <td>CAP_SURFACE3D_COUNT </td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Maximum number of CmSurface3D that can co-exist at any
    //!                      time in a CmDevice </td>
    //!   </tr>
    //!   <tr>
    //!     <td>CAP_SURFACE_COUNT_PER_KERNEL </td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Maximum number of surfaces (including 1D, 2D and 3D) that
    //!                    one kernel can use </td>
    //!    </tr>
    //!    <tr>
    //!      <td>CAP_ARG_COUNT_PER_KERNEL </td>
    //!      <td>4</td>
    //!      <td>uint32_t</td>
    //!      <td>Maximum number of arguments that a kernel can have </td>
    //!    </tr>
    //!    <tr>
    //!     <td>CAP_ARG_SIZE_PER_KERNEL </td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Maximum size of all arguments that a kernel can have </td>
    //!    </tr>
    //!    <tr>
    //!     <td>CAP_USER_DEFINED_THREAD_COUNT_PER_TASK </td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Maximum number of threads that all kernels of a task can
    //!                 run, it's only used for media object usage </td>
    //!    </tr>
    //!     <tr>
    //!     <td>CAP_HW_THREAD_COUNT </td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Maximum number of threads that HW can run in parallel.This
    //!       indicates hardware parallelism and is hence one
    //!       of the factors indicating performance of the target machine </td>
    //!   <tr>
    //!     <td>CAP_SURFACE2D_FORMAT_COUNT </td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Number of DDI formats supported for CmSurface2D creation </td>
    //!   <tr>
    //!     <td>CAP_SURFACE2D_FORMATS </td>
    //!     <td>sizeof(DDIFORMAT) * CAP_SURFACE2D_FORMAT_COUNT</td>
    //!     <td>Array of DDIFORMAT</td>
    //!     <td>All DDI format supported to create CmSurface2D </td>
    //!   <tr>
    //!     <td>CAP_SURFACE3D_FORMAT_COUNT </td>
    //!     <td>sizeof(DDIFORMAT) * CAP_SURFACE3D_FORMAT_COUNT</td>
    //!     <td>Array of DDIFORMAT</td>
    //!     <td>Number of DDI formats supported for CmSurface3D creation </td>
    //!   <tr>
    //!     <td>CAP_SURFACE3D_FORMATS </td>
    //!     <td>sizeof(DDIFORMAT) * CAP_SURFACE3D_FORMAT_COUNT</td>
    //!     <td>Array of DDIFORMAT</td>
    //!     <td>All DDI format supported to create CmSurface3D </td>
    //!   <tr>
    //!     <td>CAP_VME_STATE_COUNT </td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Maximum number of VME states that can co - exist
    //!                at any time in a CmDevice </td>
    //!   <tr>
    //!     <td>CAP_GPU_PLATFORM </td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Return GPU platform as an enum of type GPU_PLATFORM
    //!                  (PLATFORM_INTEL_SNB, PLATFORM_INTEL_IVB, etc) </td>
    //!   <tr>
    //!     <td>CAP_GT_PLATFORM </td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Return GT platform (SKU) as an enum of type GPU_GT_PLATFORM
    //!            (PLATFORM_INTEL_GT1, PLATFORM_INTEL_GT2, etc) </td>
    //!   <tr>
    //!     <td>CAP_MIN_FREQUENCY </td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Returns the minimum frequency of the GPU as an integer in MHz
    //!     </td>
    //!   <tr>
    //!     <td>CAP_MAX_FREQUENCY </td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Returns the maximum frequency of the GPU as an integer in MHz
    //!     </td>
    //!   <tr>
    //!     <td>CAP_GPU_CURRENT_FREQUENCY </td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Return the current frequency of the GPU as an integer
    //!              in MHz </td>
    //!   </tr>
    //!   <tr>
    //!     <td>CAP_USER_DEFINED_THREAD_COUNT_PER_TASK_NO_THREAD_ARG </td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Returns the maximum thread count on media object without
    //!          per - thread argument </td>
    //!   </tr>
    //!   <tr>
    //!     <td>CAP_USER_DEFINED_THREAD_COUNT_PER_MEDIA_WALKER </td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Returns the maximum thread count in media walker usage </td>
    //!   </tr>
    //!   <tr>
    //!     <td>CAP_USER_DEFINED_THREAD_COUNT_PER_THREAD_GROUP </td>
    //!     <td>4</td>
    //!     <td>uint32_t</td>
    //!     <td>Returns the maximum thread count per thread group in
    //!             GPGPU walker usage </td>
    //!   </tr>
    //!   <tr>
    //!     <td>CAP_SURFACE2DUP_COUNT </td>
    //!     <td>4</td>
    //!   <td>uint32_t</td>
    //!     <td>Maximum number of CmSurface2DUP that can
    //!           co - exist at any time </td>
    //!   </tr>
    //! </table>
    //!
    CM_RT_API virtual int32_t GetCaps(CM_DEVICE_CAP_NAME capName,
                                      uint32_t &capValueSize,
                                      void *capValue) = 0;

    //!
    //! \brief      Creates a CmThreadSpace object.
    //! \details    CmThreadSpace is a 2D space.Each unit is notated as
    //!             a pair of X/Y coordinates, which is in the range of [0, width -1]
    //!             or [0, heigh-1]. A thread space can define a dependency or no
    //!             dependency. A thread space can be used as per-task thread space
    //!             by passing it in Enqueue(), or be used as per-kernel thread space
    //!             by calling CmKernel::AssociateThreadSpace() API. Please
    //!             refer to "Host programming guide" for detailed thread space usages.
    //! \param      [in] width
    //!             Thread space width.
    //! \param      [in] height
    //!             Thread space height.
    //! \param      [out] threadSpace
    //!             Reference to pointer to CmThreadSpace object to be created.
    //! \retval     CM_SUCCESS if the CmThreadSpace is successfully created.
    //! \retval     CM_INVALID_THREAD_SPACE if the width or(and) height are
    //! \retval     invalid values (0 or exceeds maximum size).
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of system memory.
    //! \retval     CM_FAILURE otherwise.
    //! \note       The maximum width/height allowed when using media walker is
    //!             511 for pre-SKL and 2047 for SKL+. For media object the
    //!             maximum width/height allowed is 512.
    //!
    CM_RT_API virtual int32_t CreateThreadSpace(uint32_t width,
                                                uint32_t height,
                                                CmThreadSpace* &threadSpace) = 0;

    //!
    //! \brief      Creates a CmBufferUP object.
    //! \details    This API creates a CmBufferUP object on top of the UP
    //!             (User Provided) system memory with specificed size in bytes.
    //!             The UP memory starting address must be page (4K Bytes) aligned.
    //! \param      [in] size
    //!             BufferUP size in bytes, the valid range is:
    //!             > CM_MIN_SURF_WIDTH, and < CM_MAX_1D_SURF_WIDTH.
    //! \param      [in] pSystMem
    //!             Pointer to the system memory.
    //! \param      [out] surface
    //!             Reference to the pointer to the CmBufferUP.
    //! \retval     CM_SUCCESS if the CmBufferUP is successfully created.
    //! \retval     CM_SURFACE_ALLOCATION_FAILURE if creating the underneath 1D
    //!             surface fails.
    //! \retval     CM_INVALID_WIDTH if width is less than CM_MIN_SURF_WIDTH or
    //!             larger than CM_MAX_1D_SURF_WIDTH.
    //! \retval     CM_INVALID_ARG_VALUE if pSystMem is nullptr.
    //! \retval     CM_EXCEED_SURFACE_AMOUNT if maximum amount of 1D surfaces
    //!             is exceeded. The amount is the amount of the surfaces
    //!             that can co-exist. The amount can be obtained by
    //!             querying the cap CAP_BUFFER_COUNT.
    //! \retval     CM_FAILURE otherwise.
    //! \note       Application can access the memory though the memory
    //!             point from CPU; can also access the
    //!             buffer created upon the same memory from GPU. It is
    //!             application's responsibility to make
    //!             sure accesses from both sides are not overlapped.
    //! \note       Refer to "MDF Host Programming Guide" for detailed usages.
    //!
    CM_RT_API virtual int32_t CreateBufferUP(uint32_t size,
                                             void *sysMem,
                                             CmBufferUP* &surface) = 0;

    //!
    //! \brief      Destroys CmBufferUP object.
    //! \details    The UP (User Provided) memory is still existing after the
    //!             CmBufferUP object is destroyed.
    //! \param      [in, out] surface
    //!             Reference to the pointer pointing to CmBufferUP. It will be
    //!             assigned to nullptr once the function is returned.
    //! \retval     CM_SUCCESS if CmBufferUP is successfully destroyed.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t DestroyBufferUP(CmBufferUP* &surface) = 0;

    //!
    //! \brief      Forces the BufferUP object to be destroyed.
    //! \param      [in, out] surface
    //!             Reference to the pointer pointing to CmBufferUP. It will be
    //!             assigned to nullptr once the function is returned.
    //! \retval     CM_SUCCESS if CmBufferUP is successfully destroyed.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t ForceDestroyBufferUP(CmBufferUP* &surface) = 0;

    //!
    //! \brief      Gets Surface2D allocation information by given width,
    //!             height, and format.
    //! \details    Gets necessary information in order to create and use
    //!             CmSurface2DUP.
    //!             To create CmSurface2DUP, user needs to allocated such
    //!             amount of system memory which equals to
    //!             or larger than physical size returned here. When
    //!             accessing the system memory, the user needs be
    //!             aware about pitch, which is equal to (pixel_width *
    //!             byte_per_pixel + necessary_padding).
    //! \param      [in] width
    //!             Width in pixel.
    //! \param      [in] height
    //!             height in pixel.
    //! \param      [in] format
    //!             pixel format.
    //! \param      [out] pitch
    //!             Reference to returned pitch.
    //! \param      [out] physicalSize
    //!             Reference to returned physical size.
    //! \retval     CM_SUCCESS always
    //!
    CM_RT_API virtual int32_t GetSurface2DInfo(uint32_t width,
                                               uint32_t height,
                                               CM_SURFACE_FORMAT format,
                                               uint32_t &pitch,
                                               uint32_t &physicalSize) = 0;

    //!
    //! \brief      Creates a CmSurface2DUP object.
    //! \details    Creates a CmSurface2DUP in UP (User Provided) system memory
    //!             with given surface width, height in pixel, and format.
    //!             The UP system memory must be page (4K Bytes) aligned.
    //!             The size of the system memory must larger than or equal to
    //!             the size return by GetSurface2DInfo().
    //! \param      [in] width
    //!             Width in pixel.
    //! \param      [in] height
    //!             Height in pixel.
    //! \param      [in] format
    //!             Format.
    //! \param      [in] sysMem
    //!             Reference to the pointer to the system memory which is CPU
    //!             accessible.
    //! \param      [out] surface
    //!             Reference to the pointer to the CmSurface2DUP.
    //! \retval     CM_SUCCESS if the CmSurface2DUPis successfully created.
    //! \retval     CM_INVALID_ARG_VALUE if sysMem is nullptr.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of system memory.
    //! \retval     CM_INVALID_WIDTH if width is less than CM_MIN_SURF_WIDTH or
    //!             larger than CM_MAX_2D_SURF_WIDTH, or for YUY2 or NV12
    //!             format, the width is odd.
    //! \retval     CM_INVALID_HEIGHT if height is less than CM_MIN_SURF_HEIGHT
    //!             or larger than CM_MAX_2D_SURF_HEIGHT, or for NV12 format,
    //!             the height is odd.
    //! \retval     CM_SURFACE_FORMAT_NOT_SUPPORTED if the format is not
    //!             supported. The supported formats can be obtained by
    //!             querying cap CAP_SURFACE2D_FORMAT_COUNT
    //!             and CAP_SURFACE2D_FORMATS.
    //! \retval     CM_EXCEED_SURFACE_AMOUNT if maximum amount of 2D surfaces
    //!             is exceeded. The amount is the amount of the surfaces that
    //!             can co-exist. The amount can be obtained by
    //!             querying the cap CAP_SURFACE2D_COUNT.
    //! \retval     CM_SURFACE_ALLOCATION_FAILURE if allocation is failed.
    //! \retval     CM_FAILURE otherwise.
    //! \note       Application can access the memory though the memory point
    //!             returned from CPU; can also access the surface created
    //!             upon the same memory from GPU. It is application's
    //!             responsibility to make sure accesses from both sides are
    //!             not overlapped. When accessing the system memory from CPU,
    //!             the user needs to be aware about pitch, which is equal to
    //!             (pixel_width * byte_per_pixel + necessary_padding).
    //! \note       Refer to the CmSurface2DUP class for member APIs, and
    //!             'MDF runtime host programming guide' for usages.
    //!
    CM_RT_API virtual int32_t CreateSurface2DUP(uint32_t width,
                                                uint32_t height,
                                                CM_SURFACE_FORMAT format,
                                                void *sysMem,
                                                CmSurface2DUP* &surface) = 0;

    //!
    //! \brief      Destroys CmSurface2DUP surface.
    //! \details    The UP (User Provided) memory is still existing after the
    //!             CmSurface2DUP object is destroyed.
    //! \param      [in] surface
    //!             Reference to the pointer pointing to CmSurface2DUP. It will
    //!             be assigned to nullptr once this function is returned.
    //! \retval     CM_SUCCESS if CmSurface2DUPis successfully destroyed.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t
    DestroySurface2DUP(CmSurface2DUP* &surface) = 0;

    //!
    //! \brief      Creates a VME surface for AVC messages in kernel.
    //! \details    Creates a VME surface by using the given 2D surfaces:
    //!             the current frame, the forward frames and, the backward
    //!             frames. The last two can be nullptr if not used. The function
    //!             indicates these 2D surfaces are used for VME; no extra
    //!             surface is actually created. A SurfaceIndex object is created
    //!             instead, which is passed to CM kernel function (genx_main)
    //!             as argument to indicate the frame surface. Please see VME
    //!             examples in "MDF Host Programming Guide" document and CM
    //!             language specification for details.
    //! \param      [in] currentSurface
    //!             Pointer to current surface (can't be nullptr).
    //! \param      [in] forwardSurfaces
    //!             Array of forward surfaces (can be nullptr).
    //! \param      [in] backwardSurfaces
    //!             Array of backward surfaces (can be nullptr).
    //! \param      [in] forwardSurfaceCount
    //!             Count of forward surfaces, up to 16 forward surfaces can be
    //!             used.
    //! \param      [in] backwardSurfaceCount
    //!             Count of backward surfaces, up to 16 backward surfaces can
    //!             be used.
    //! \param      [out] vmeIndex
    //!             Reference to pointer to SurfaceIndex object to be created.
    //! \retval     CM_SUCCESS if the SurfaceIndex is successfully created.
    //! \retval     CM_NULL_POINTER if currentSurface is nullptr.
    //! \retval     CM_INVALID_ARG_VALUE if any parameter is not valid.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of system memory.
    //! \retval     CM_EXCEED_SURFACE_AMOUNT if maximum amount of VME surfaces
    //!             is exceeded. The amount is the amount of VME surfaces that
    //!             can co-exist. The amount can be obtained by querying the
    //!             cap CAP_VME_SURFACE_COUNT.
    //! \retval     CM_FAILURE otherwise.
    //! \note       This can be used for all Gen7_5 and plus platforms.
    //!
    CM_RT_API virtual int32_t
    CreateVmeSurfaceG7_5(CmSurface2D *currentSurface,
                         CmSurface2D **forwardSurfaces,
                         CmSurface2D **backwardSurfaces,
                         const uint32_t forwardSurfaceCount,
                         const uint32_t backwardSurfaceCount,
                         SurfaceIndex* &vmeIndex) = 0;

    //!
    //! \brief      Destroys a VME surface object.
    //! \details    Any VME surface not destroyed by calling this function
    //!             explicitly will be destroyed when CmDevice is destroyed.
    //! \param      [in] vmeIndex
    //!             Pointer to the SurfaceIndex of the VME surface. It will be
    //!             assigned to nullptr once destroy is done.
    //! \retval     CM_SUCCESS if the VME surface is successfully destroyed.
    //! \retval     CM_FAILURE otherwise.
    //! \note       This can be used for all Gen7_5 and plus platforms.
    //!
    CM_RT_API virtual int32_t
    DestroyVmeSurfaceG7_5(SurfaceIndex* &vmeIndex) = 0;

    //!
    //! \brief      Creates a CmSampler8x8 object.
    //! \param      [in] sampler8x8Descriptor
    //!             Const reference to a CM_SAMPLER_8X8_DESCR specifying the
    //!             characteristics of the Sampler8x8 state to be created.
    //!             Currently, AVS, VA Convolve and VA Misc( including MinMax
    //!             Filter/Erode/Dilate ) states are supported.
    //! \param      [out] sampler8x8
    //!             Reference to the pointer to the CmSampler8x8 object.
    //! \retval     CM_SUCCESS if the CmSampler8x8is successfully created.
    //! \retval     CM_INVALID_ARG_VALUE wrong sampler8x8 type.
    //! \retval     CM_EXCEED_SAMPLER_AMOUNT if the co-existed sampler exceeds
    //!             maximum count which can be queried by CAP_SAMPLER_COUNT cap.
    //!
    CM_RT_API virtual int32_t
    CreateSampler8x8(const CM_SAMPLER_8X8_DESCR &sampler8x8Descriptor,
                     CmSampler8x8* &sampler8x8) = 0;

    //!
    //! \brief      Destroys a CmSampler8x8 object.
    //! \details    A CmSampler8x8 which is not destroyed by calling this
    //!             function will be destroyed when the CmDevice is destroyed.
    //! \param      [in, out] sampler8x8State
    //!             Reference to a sampler of CmSampler8x8. It will be assigned
    //!             to nullptr once destroy is done.
    //! \retval     CM_SUCCESS if the CmSampler8x8 is successfully destroyed.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t DestroySampler8x8(CmSampler8x8* &sampler8x8State) = 0;

    //!
    //! \brief      Creates a CmSampler8x8 surface.
    //! \details    Creates a CmSampler8x8 surface by using the given 2D surface.
    //!             The function indicates the 2D surface is used for sampler
    //!             8x8; no extra surface is actually created. A SurfaceIndex
    //!             object is created instead, which is passed to CM kernel
    //!             function (genx_main) as argument to indicate the surface
    //!             for sampler 8x8.
    //! \param      [in] surface2d
    //!             Pointer to CmSurface2D.
    //! \param      [out] sampler8x8SurfIndex
    //!             Reference to pointer to SurfaceIndex.
    //! \param      [in] surfType
    //!             Enumeration data type of CM_SAMPLER8x8_SURFACE.
    //! \param      [in] mode
    //!             Enumeration data type of CM_SURFACE_ADDRESS_CONTROL_MODE.
    //! \retval     CM_SUCCESS if the CmSampler8x8 surface is successfully
    //!             created.
    //! \retval     CM_EXCEED_SURFACE_AMOUNT if there is too many co-existed
    //!             surfaces and exceed the maximum number. Destroying some
    //!             unused surfaces could solve this error.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of host memory
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t
    CreateSampler8x8Surface(CmSurface2D *surface2d,
                            SurfaceIndex* &sampler8x8SurfIndex,
                            CM_SAMPLER8x8_SURFACE surfType,
                            CM_SURFACE_ADDRESS_CONTROL_MODE mode) = 0;

    //!
    //! \brief      Destroys a CmSampler8x8 surface.
    //! \details    A CmSampler8x8 surface which is not destroyed by calling
    //!             this function will be destroyed when the CmDevice is
    //!             destroyed.
    //! \param      [in] sampler8x8SurfIndex
    //!             Reference to SurfaceIndex. It will be assigned to nullptr
    //!             once destroy is done.
    //! \retval     CM_SUCCESS if the CmSampler8x8 surface is successfully
    //!             destroyed.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t
    DestroySampler8x8Surface(SurfaceIndex* &sampler8x8SurfIndex) = 0;

    //!
    //! \brief      Creates a 2-dimensional thread group space object.
    //! \details    This function creates a thread group space specified by the
    //!             height and width dimensions of the group space, and the
    //!             height and width dimensions of the thread space within a
    //!             group. This information is used to execute a kernel in GPGPU pipe
    //!             (https://01.org/sites/default/files/documentation/intel-gfx-prm-osrc-skl-vol07-3d_media_gpgpu.pdf).
    //!             Relevant sample code is shown in "MDF Host Programming Guide"
    //! \param      [in] threadSpaceWidth
    //!             width in unit of threads of each thread group.
    //! \param      [in] threadSpaceHeight
    //!             height in unit of threads of each thread group.
    //! \param      [in] groupSpaceWidth
    //!             width in unit of groups of thread group space.
    //! \param      [in] groupSpaceHeight
    //!             height in unit of groups of thread group space.
    //! \param      [out] threadGroupSpace
    //!             Reference to the pointer to CmThreadGroupSpace object to be
    //!             created.
    //! \retval     CM_SUCCESS if the CmThreadGroupSpace is successfully
    //!             created.
    //! \retval     CM_INVALID_THREAD_GROUP_SPACE if any input is 0 or the
    //!             thrdSpaceWidth is more than MAX_THREAD_SPACE_WIDTH_PERGROUP,
    //!             or the thrdSpaceHeight is more than
    //!             MAX_THREAD_SPACE_HEIGHT_PERGROUP.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of host memory.
    //! \retval     CM_FAILURE otherwise.
    //! \note       The total thread count is width*height*grpWidth*grpHeight.
    //!             CmKernel::SetThreadCount() calling is not necessary in this
    //!             GPGPU working mode. Currently, it's only used for SLM
    //!             enabled kernels. See also CreateThreadGroupSpaceEx() API which
    //!             specifies a 3-dimensional thread group space with width, height
    //!             and depth.
    //!
    CM_RT_API virtual int32_t
    CreateThreadGroupSpace(uint32_t threadSpaceWidth,
                           uint32_t threadSpaceHeight,
                           uint32_t groupSpaceWidth,
                           uint32_t groupSpaceHeight,
                           CmThreadGroupSpace* &threadGroupSpace) = 0;

    //!
    //! \brief      Destroys the created thread group space object.
    //! \details    Caller provides the reference of thread group space pointer
    //! \param      [in] threadGroupSpace
    //!             Pointer to a CmThreadGroupSpace. It will be assigned to
    //!             nullptr once destroy is done.
    //! \retval     CM_SUCCESS if the CmThreadGroupSpace pointer is
    //!             successfully destroyed.
    //! \note       User can call this API to explicitly destroy
    //!             CmThreadGroupSpace instance, otherwise, DestroyCmDevice()
    //!             takes care of all of such instances release.
    //!
    CM_RT_API virtual int32_t
    DestroyThreadGroupSpace(CmThreadGroupSpace* &threadGroupSpace) = 0;

    //!
    //! \brief      Sets the configuration for L3 cache.
    //! \details    This API allows users to configure L3 cach by themselves.
    //! \param      [in] l3Config
    //!             L3ConfigRegisterValues contains the values of L3 control
    //!             registers. The registers are different from platform to
    //!             platform. \n
    //!             struct L3ConfigRegisterValues \n
    //!             { \n
    //!             \t unsigned int config_register0; \n
    //!             \t unsigned int config_register1; \n
    //!             \t unsigned int config_register2; \n
    //!             \t unsigned int config_register3; \n
    //!             }; \n
    //! \retval     CM_SUCCESS if the L3 configuration pointer is set correctly.
    //! \retval     CM_FAILURE if the platform does not support L3 or L3
    //!             configuration failed to be set.
    //! \note       This function is implemented for both hardware mode and
    //!             simulation mode.
    //!
    CM_RT_API virtual int32_t SetL3Config(const L3ConfigRegisterValues *l3Config) = 0;

    //!
    //! \brief      Sets the suggested configuration for L3 cache.
    //! \param      [in] l3SuggestConfig
    //!             The configuration plan which represents a suggested L3
    //!             configuration. These configurations are defined in
    //!             ::L3_SUGGEST_CONFIG which is a enumeration definition.
    //! \retval     CM_SUCCESS if the L3 configuration pointer is set correctly.
    //! \retval     CM_FAILURE if the platform does not support L3 or L3
    //!             configuration failed to be set.
    //! \note       This function is only implemented for hardware and
    //!             simulation modes.
    //!
    CM_RT_API virtual int32_t
    SetSuggestedL3Config(L3_SUGGEST_CONFIG l3SuggestConfig) = 0;

    //!
    //! \brief      This function can be used to set/limit hardware
    //!             capabilities- number of threads that HW can run in parallel
    //! \details    Hardware thread number can be set from 1 to maximum.
    //! \param      [in] capName
    //!             Name of cap to set.
    //! \param      [in] capValueSize
    //!             The size of the cap value.
    //! \param      [in] capValue
    //!             Pointer to the cap value.
    //! \retval     CM_SUCCESS if cap value is valid and is set correctly.
    //! \retval     CM_INVALID_HARDWARE_THREAD_NUMBER specific SetCaps error
    //!             message if cap value is not valid.
    //! \retval     CM_NOT_IMPLEMENTED for emulation mode.
    //! \retval     CM_FAILURE otherwise.
    //! \note       The following is the specific behavior for the cap value
    //!             that is being set.
    //!             <table>
    //!                <tr>
    //!                 <th>Cap name< / th>
    //!                 <th>Behavior< / th>
    //!                < / tr>
    //!                <tr>
    //!                 <td>CAP_HW_THREAD_COUNT< / td>
    //!                 <td>The number of hardware threads is per - task.A call
    //!                 to the SetCaps function to set the CAP_HW_THREAD_COUNT
    //!                 will limit the maximum number of hardware threads for
    //!                 the ensuing call to Enqueue.After the call to Enqueue,
    //!                 the maximum number of hardware threads will be restored
    //!                 to its default value, which is determined by the
    //!                 hardware's capability.
    //!                 < / td>
    //!                < / tr>
    //!             < / table>
    //!
    CM_RT_API virtual int32_t SetCaps(CM_DEVICE_CAP_NAME capName,
                                      size_t capValueSize,
                                      void *capValue) = 0;

    //!
    //! \brief      This function creates a sampler surface index by a given
    //!             CmSurface2D.
    //! \details    This sampler surface doesn't create any actual surface. It
    //!             just binds the actual 2D surface with a virtual sampler
    //!             surface index. User need pass this surface index as kernel
    //!             argument if the surface is used for sampler, otherwise,
    //!             the runtime will report error if user pass the 2D surface
    //!             index. For the 2D surface format, for now  supports
    //!             following formats: \n
    //!                 CM_SURFACE_FORMAT_A16B16G16R16 \n
    //!                 CM_SURFACE_FORMAT_A16B16G16R16F \n
    //!                 CM_SURFACE_FORMAT_R32G32B32A32F \n
    //!                 CM_SURFACE_FORMAT_A8 \n
    //!                 CM_SURFACE_FORMAT_A8R8G8B8 \n
    //!                 CM_SURFACE_FORMAT_YUY2 \n
    //!                 CM_SURFACE_FORMAT_R32F \n
    //!                 CM_SURFACE_FORMAT_R32_UINT \n
    //!                 CM_SURFACE_FORMAT_L16 \n
    //!                 CM_SURFACE_FORMAT_R16G16_UNORM \n
    //!                 CM_SURFACE_FORMAT_R16_FLOAT \n
    //!                 CM_SURFACE_FORMAT_NV12 \n
    //!                 CM_SURFACE_FORMAT_L8 \n
    //!                 CM_SURFACE_FORMAT_AYUV \n
    //!                 CM_SURFACE_FORMAT_Y410 \n
    //!                 CM_SURFACE_FORMAT_Y416 \n
    //!                 CM_SURFACE_FORMAT_Y210 \n
    //!                 CM_SURFACE_FORMAT_Y216 \n
    //!                 CM_SURFACE_FORMAT_P010 \n
    //!                 CM_SURFACE_FORMAT_P016 \n
    //!                 CM_SURFACE_FORMAT_YV12 \n
    //!                 CM_SURFACE_FORMAT_411P \n
    //!                 CM_SURFACE_FORMAT_411R \n
    //!                 CM_SURFACE_FORMAT_IMC3 \n
    //!                 CM_SURFACE_FORMAT_I420 \n
    //!                 CM_SURFACE_FORMAT_422H \n
    //!                 CM_SURFACE_FORMAT_422V \n
    //!                 CM_SURFACE_FORMAT_444P \n
    //!                 CM_SURFACE_FORMAT_RGBP \n
    //!                 CM_SURFACE_FORMAT_BGRP \n
    //! \param      [in] surface2d
    //!             Pointer to CmSurface2D object.
    //! \param      [out] samplerSurfaceIndex
    //!             Reference to the pointer to SurfaceIndex object to be
    //!             created.
    //! \retval     CM_SUCCESS if the new sampler surface index is successfully
    //!             created.
    //! \retval     CM_SURFACE_FORMAT_NOT_SUPPORTED if the input surface format
    //!             is not list above.
    //! \retval     CM_EXCEED_SURFACE_AMOUNT if the total number of
    //!             co-existed surfaces are exceed maximum count.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of host memory.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t
    CreateSamplerSurface2D(CmSurface2D *surface2d,
                           SurfaceIndex* &samplerSurfaceIndex) = 0;

    //!
    //! \brief      This function creates a sampler surface index by a given
    //!             CmSurface3D.
    //! \details    This function call doesn't create any actual surface. It
    //!             just binds the actual 3D surface with a virtual sampler
    //!             surface index. User need pass this surface index as kernel
    //!             argument if the surface is used for sampler, otherwise, the
    //!             runtime will report error if user pass the 3D surface
    //!             index. For the 3D surface format, for now only supports the
    //!             CM_SURFACE_FORMAT_A8R8G8B8 and CM_SURFACE_FORMAT_A16B16G16R16
    //!             formats.
    //! \param      [in] surface3d
    //!             Pointer to CmSurface3D object.
    //! \param      [out] samplerSurfaceIndex
    //!             Reference to the pointer to SurfaceIndex object to be
    //!             created.
    //! \retval     CM_SUCCESS if the sampler surface index is successfully
    //!             created.
    //! \retval     CM_SURFACE_FORMAT_NOT_SUPPORTED if the input surface format
    //!             is not list above.
    //! \retval     CM_EXCEED_SURFACE_AMOUNT if the total number of created
    //!             co-existed surfaces are exceed maximum count.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of host memory.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t
    CreateSamplerSurface3D(CmSurface3D *surface3d,
                           SurfaceIndex* &samplerSurfaceIndex) = 0;

    //!
    //! \brief      This function destroys a sampler surface index created by
    //!             CreateSamplerSurface2D(), CreateSamplerSurface2DUP, or
    //!             CreateSamplerSurface3D().
    //! \details    Caller provides the reference of a pointer to the surface
    //!             index needs t be destoryed.
    //! \param      [in] samplerSurfaceIndex
    //!             Reference to the pointer to SurfaceIndex object to be
    //!             destroyed.
    //! \retval     CM_SUCCESS if the sampler surface index is successfully
    //!             destroyed.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t
    DestroySamplerSurface(SurfaceIndex* &samplerSurfaceIndex) = 0;

    //!
    //! \brief      This function creates a buffer to store the message printed
    //!             by printf() in kernel side.
    //! \details    The default size of print buffer is 1M bytes. User can set
    //!             its size according to the length of message printed in
    //!             kernel and the number of threads. printf() can be used for
    //!             kernel debug purpose
    //! \param      [in] size
    //!             The size of print buffer in bytes.
    //! \retval     CM_SUCCESS if the print buffer is created successfully.
    //! \retval     CM_OUT_OF_HOST_MEMORY if print buffer allication is failed.
    //! \retval     CM_FAILURE otherwise.
    //! \note       Internally the print buffer occupies static buffer index 1,
    //!             thus only other 3 static buffers can be used by host (0, 2, 3)
    //!             if print functionality is enabled.
    //!
    CM_RT_API virtual int32_t
    InitPrintBuffer(size_t size = CM_DEFAULT_PRINT_BUFFER_SIZE) = 0;

    //!
    //! \brief      This function prints the message on the standard display
    //!             device that are dumped by kernel.
    //! \details    It should be called after the task being finished. The
    //!             order of printf output is not deterministic due to thread
    //!             scheduling and the fact that different threads may be
    //!             interleaved. To distinguish which thread the printf string
    //!             comes from, it is better to print the thread id as the
    //!             first value. Alternatively you could always
    //!             put the printf inside if statement that limits the printf to a
    //!             given thread. If one task has more than one kernels call
    //!             printf() , their outputs could mix together.
    //! \retval     CM_SUCCESS if the buffer is flushed successfully.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t FlushPrintBuffer() = 0;

    //!
    //! \brief      This function creates a VEBOX object for VEBOX
    //!             operations (https://01.org/sites/default/files/documentation/intel-gfx-prm-osrc-skl-vol09-media_vebox.pdf).
    //! \details    Caller provides a reference of CmVebox pointer to get the
    //!             CmVebox object created from this function.
    //! \param      [in, out] vebox
    //!             the created VEBOX object.
    //! \retval     CM_SUCCESS if creation is successfully.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of host memory.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t CreateVebox(CmVebox* &vebox) = 0;

    //!
    //! \brief      This function destroys a VEBOX object.
    //! \details    Caller provides a reference of CmVebox pointer to destroy.
    //! \param      [in, out] vebox
    //!             The VEBOX object to be destroyed. It will be assigned to
    //!             nullptr once destroy is done.
    //! \retval     CM_SUCCESS if creation is successfully.
    //! \retval     CM_NULL_POINTER if the vebox pointer is nullptr.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t DestroyVebox(CmVebox* &vebox) = 0;

    //!
    //! \brief      It creates a CmBufferSVM of the specified size in bytes by
    //!             using the SVM (shared virtual memory) system memory.
    //!             (https://01.org/sites/default/files/documentation/intel-gfx-prm-osrc-skl-vol05-memory_views.pdf).
    //! \details    The SVM memory can be accessed by both CPU and GPU. The SVM
    //!             memory will be allocated in runtime internally(if user pass
    //!             nullptr pointer) or user provided (if user pass a valid
    //!             pointer). In both way, the memory should be page aligned
    //!             (4K bytes). And the staring address is returned.
    //! \param      [in] size
    //!             SVM buffer size in bytes.
    //! \param      [in,out] pSystMem
    //!             Pointer to the SVM memory starting address.
    //! \param      [in] accessFlag
    //!             Buffer access flags.
    //! \param      [out] bufferSVM
    //!             Reference to the pointer to the CmBufferSVM.
    //! \retval     CM_SUCCESS if the CmBufferSVM is successfully created.
    //! \retval     CM_SURFACE_ALLOCATION_FAILURE if creating the underneath 1D
    //!             surface fails.
    //! \retval     CM_INVALID_WIDTH if width is less than CM_MIN_SURF_WIDTH or
    //!             larger than CM_MAX_1D_SURF_WIDTH.
    //! \retval     CM_OUT_OF_HOST_MEMORY if runtime can't allocate such size
    //!             SVM memory.
    //! \retval     CM_EXCEED_SURFACE_AMOUNT if maximum amount of 1D surfaces
    //!             is exceeded. The amount is the amount of the surfaces that
    //!             can co-exist. The amount can be obtained by querying the
    //!             cap CAP_BUFFER_COUNT.
    //! \retval     CM_FAILURE otherwise.
    //! \note       This API is not implemented in Linux for now.
    //!
    CM_RT_API virtual int32_t CreateBufferSVM(uint32_t size,
                                              void* &sysMem,
                                              uint32_t accessFlag,
                                              CmBufferSVM* &bufferSVM) = 0;

    //!
    //! \brief      Destroys CmBufferSVM object and associated SVM memory.
    //! \param      [in,out] bufferSVM
    //!             Reference to the pointer pointing to CmBufferSVM, will be
    //!             assigned to nullptr once it is destroyed successfully.
    //! \retval     CM_SUCCESS if CmBufferSVM and associated SVM meory are
    //!             successfully destroyed.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t DestroyBufferSVM(CmBufferSVM* &bufferSVM) = 0;

    //!
    //! \brief      This function creates a sampler surface index by a
    //!             CmSurface2DUP.
    //! \details    This sampler surface doesn't create any actual surface,
    //!             and just bind the actual 2D UP (User Provided) surface
    //!             with a virtual sampler surface index. User need pass this
    //!             surface index as kernel argument if the surface is used
    //!             for sampler, otherwise, the runtime will report error if
    //!             user pass the 2D UP surface index. For the 2DUP surface
    //!             formats, for now  supports following formats: \n
    //!                 CM_SURFACE_FORMAT_A16B16G16R16 \n
    //!                 CM_SURFACE_FORMAT_A8 \n
    //!                 CM_SURFACE_FORMAT_A8R8G8B8 \n
    //!                 CM_SURFACE_FORMAT_YUY2 \n
    //!                 CM_SURFACE_FORMAT_R32F \n
    //!                 CM_SURFACE_FORMAT_R32_UINT \n
    //!                 CM_SURFACE_FORMAT_L16 \n
    //!                 CM_SURFACE_FORMAT_R16G16_UNORM \n
    //!                 CM_SURFACE_FORMAT_NV12 \n
    //!                 CM_SURFACE_FORMAT_L8 \n
    //!                 CM_SURFACE_FORMAT_AYUV \n
    //!                 CM_SURFACE_FORMAT_Y410 \n
    //!                 CM_SURFACE_FORMAT_Y416 \n
    //!                 CM_SURFACE_FORMAT_Y210 \n
    //!                 CM_SURFACE_FORMAT_Y216 \n
    //!                 CM_SURFACE_FORMAT_P010 \n
    //!                 CM_SURFACE_FORMAT_P016 \n
    //!                 CM_SURFACE_FORMAT_YV12 \n
    //!                 CM_SURFACE_FORMAT_411P \n
    //!                 CM_SURFACE_FORMAT_411R \n
    //!                 CM_SURFACE_FORMAT_IMC3 \n
    //!                 CM_SURFACE_FORMAT_I420 \n
    //!                 CM_SURFACE_FORMAT_422H \n
    //!                 CM_SURFACE_FORMAT_422V \n
    //!                 CM_SURFACE_FORMAT_444P \n
    //!                 CM_SURFACE_FORMAT_RGBP \n
    //!                 CM_SURFACE_FORMAT_BGRP \n
    //! \param      [in] surface2dUP
    //!             Pointer to CmSurface2DUP object.
    //! \param      [out] samplerSurfaceIndex
    //!             Reference to the pointer to SurfaceIndex object to be
    //!             created.
    //! \retval     CM_SUCCESS if the new sampler surface index is
    //!             successfully created.
    //! \retval     CM_NULL_POINTER if p2DUPSurface is nullptr.
    //! \retval     CM_SURFACE_FORMAT_NOT_SUPPORTED if the format is not supported.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of host memory.
    //! \retval     CM_FAILURE otherwise.
    //! \note       This API is supported for HW mode only.
    //!
    CM_RT_API virtual int32_t
    CreateSamplerSurface2DUP(CmSurface2DUP *surface2dUP,
                             SurfaceIndex* &samplerSurfaceIndex) = 0;

    //!
    //! \brief      Copies the content of source kernel to a new kernel.
    //! \param      [out] kernelDst
    //!             pointer to the destination kernel. The new pointer will be
    //!             returned to kernelDst.
    //! \param      [in] kernelSrc
    //!             pointer to the source kernel.
    //! \retval     CM_SUCCESS If the clone operation is successful.
    //! \retval     CM_FAILURE If the clone operation is failed.
    //! \note       This API is not supported in emulation mode.
    //!
    CM_RT_API virtual int32_t CloneKernel(CmKernel* &kernelDst,
                                          CmKernel *kernelSrc) = 0;

    //!
    //! \brief      Creates an alias to CmSurface2D.
    //! \details    Returns a new surface index for this surface. This API is
    //!             used with CmSurface2D::SetSurfaceStateParam in order
    //!             to reinterpret surface for different surface states,
    //!             i.e., the same memory is used but different width and
    //!             height can be programmed through the surface state.
    //! \param      [in] p2DSurface
    //!             pointer to the surface used to create an alias.
    //! \param      [out] aliasSurfaceIndex
    //!             new surface index pointing to 2D surface.
    //! \retval     CM_SUCCESS if alias is created successfully.
    //! \retval     CM_INVALID_ARG_VALUE if p2DSurface is not a valid pointer.
    //! \retval     CM_MAX_NUM_2D_ALIASES if try to create more than 10 aliases
    //!             for same surface.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of host memory.
    //! \retval     CM_FAILURE otherwise.
    //! \note       This API is implemented for HW and SIM modes only.
    //!
    CM_RT_API virtual int32_t
    CreateSurface2DAlias(CmSurface2D *p2DSurface,
                         SurfaceIndex* &aliasSurfaceIndex) = 0;

    //!
    //! \brief      Creates an HEVC VME surface by using the given 2D surfaces:
    //!             the current frame, the forward frames and, the backward
    //!             frames.
    //! \details    No extra surface is actually created. A SurfaceIndex
    //!             object is created instead, which is passed to CM kernel
    //!             function (genx_main) as argument to indicate the frame
    //!             surface. This can be used for Gen10 and plus platforms.
    //! \param      [in] currentSurface
    //!             Pointer to current surface (can't be nullptr).
    //! \param      [in] forwardSurfaces
    //!             Array of forward surfaces (can be nullptr if backward
    //!             surfaces not a nullptr).
    //! \param      [in] backwardSurfaces
    //!             Array of backward surfaces (can be nullptr if  forward
    //!             surfaces not a nullptr).
    //! \param      [in] forwardSurfaceCount
    //!             Count of forward surfaces, up to 4 forward surfaces can
    //!             be used.
    //! \param      [in] backwardSurfaceCount
    //!             Count of backward surfaces, up to 4 backward surfaces can
    //!             be used.
    //! \param      [out] vmeIndex
    //!             Reference to pointer to SurfaceIndex object to be created.
    //! \retval     CM_SUCCESS if the SurfaceIndex is successfully created.
    //! \retval     CM_NULL_POINTER if currentSurface is nullptr.
    //! \retval     CM_INVALID_ARG_VALUE if invalid surface pointers for forward
    //!             and backward surfaces.
    //! \retval     CM_EXCEED_SURFACE_AMOUNT if there is too much co-existed
    //!             surfaces are created. Destroying unused surfaces to solve
    //!             such error.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of system memory.
    //! \retval     CM_FAILURE otherwise.
    //! \note       This API is only supported for Gen10 and plus platforms.
    //!
    CM_RT_API virtual int32_t
    CreateHevcVmeSurfaceG10(CmSurface2D *currentSurface,
                            CmSurface2D **forwardSurfaces,
                            CmSurface2D **backwardSurfaces,
                            const uint32_t forwardSurfaceCount,
                            const uint32_t backwardSurfaceCount,
                            SurfaceIndex* &vmeIndex) = 0;

    //!
    //! \brief      Destroys an HEVC VME surface. This can be used for Gen10.
    //! \param      [in] vmeIndex
    //!             Pointer to the SurfaceIndex of the VME surface. It will be
    //!             assigned to nullptr once destroy is done.
    //! \retval     CM_SUCCESS if the HEVC VME surface is successfully destroyed
    //! \retval     CM_FAILURE otherwise.
    //! \note       This API is only supported for Gen10 and plus platforms. \n
    //!             Any HEVC VME surface not destroyed by calling this function
    //!             explicitly will be destroyed when CmDevice is destroyed.
    //!
    CM_RT_API virtual int32_t
    DestroyHevcVmeSurfaceG10(SurfaceIndex* &vmeIndex) = 0;

    //!
    //! \brief      Creates a CmSampler object with border color setting.
    //! \param      [in] sampleState
    //!             Const reference to a CM_SAMPLER_STATE_EX specifying the
    //!             characteristics of the sampler to be created.
    //! \param      [out] sampler
    //!             Reference to the pointer to the CmSampler object.
    //! \retval     CM_SUCCESS if the CmSampler is successfully created.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of system memory.
    //! \retval     CM_EXCEED_SAMPLER_AMOUNT if maximum amount of sampler is
    //!             exceeded. The amount is the amount of the sampler that can
    //!             co-exist. The amount can be obtained by querying the cap
    //!             CAP_SAMPLER_COUNT.
    //! \retval     CM_FAILURE otherwise.
    //! \note       This API is not implemented for EMU mode. \n Point, linear,
    //!             and anisotropic filter types are supported for hardware and
    //!             simulation modes. \n Wrap, mirror, clamp and border are
    //!             supported in hardware and simulation modes. Clamp is
    //!             supported in emulation mode.
    //!
    CM_RT_API virtual int32_t
    CreateSamplerEx(const CM_SAMPLER_STATE_EX &sampleState,
                    CmSampler* &sampler) = 0;

    //!
    //! \brief      This function prints the message dumped by kernel into file
    //!             instead of stdout.
    //! \details    This function's usage is the same as
    //!             CmDevice::FlushPrintBuffer(). It is recommended to use this
    //!             interface when there are tons of messages from kernel.
    //! \param      [in] filename
    //!             name of file the message printed into.
    //! \retval     CM_SUCCESS if the buffer is flushed successfully into file.
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t
    FlushPrintBufferIntoFile(const char *filename) = 0;

    //!
    //! \brief      Creates a 3-dimensional thread group space object.
    //! \details    This function creates a thread group space specified by the
    //!             depth, height and width dimensions of the group space, and the
    //!             depth, height and width dimensions of the thread space within a
    //!             group. This information is used to execute a kernel in GPGPU pipe
    //!             (https://01.org/sites/default/files/documentation/intel-gfx-prm-osrc-skl-vol07-3d_media_gpgpu.pdf).
    //!             Relevant sample code is shown in "MDF Host Programming Guide".
    //! \param      [in] thrdSpaceWidth
    //!             width in unit of threads of each thread group.
    //! \param      [in] thrdSpaceHeight
    //!             height in unit of threads of each thread group.
    //! \param      [in] thrdSpaceDepth
    //!             depth in unit of threads of each thread group.
    //! \param      [in] grpSpaceWidth
    //!             width in unit of groups of thread group space.
    //! \param      [in] grpSpaceHeight
    //!             height in unit of groups of thread group space.
    //! \param      [in] grpSpaceDepth
    //!             depth in unit of groups of thread group space.
    //! \param      [out] threadGroupSpace
    //!             Reference to the pointer to CmThreadGroupSpace object to be
    //!             created.
    //! \retval     CM_SUCCESS if the CmThreadGroupSpace is successfully
    //!             created.
    //! \retval     CM_INVALID_THREAD_GROUP_SPACE if any input is 0 or the
    //!             thrdSpaceWidth is more than MAX_THREAD_SPACE_WIDTH_PERGROUP,
    //!             or the thrdSpaceHeight is more than
    //!             MAX_THREAD_SPACE_HEIGHT_PERGROUP, or the thrdSpaceDepth
    //!             is more than MAX_THREAD_SPACE_DEPTH_PERGROUP.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of host memory.
    //! \retval     CM_FAILURE otherwise.
    //! \note       The total thread count is width*height*depth*grpWidth*grpHeight*grpDepth.
    //!             CmKernel::SetThreadCount() calling is not necessary in this
    //!             GPGPU working mode. Currently, it's only used for SLM
    //!             enabled kernels.
    //!
    CM_RT_API virtual int32_t
    CreateThreadGroupSpaceEx(uint32_t thrdSpaceWidth,
                             uint32_t thrdSpaceHeight,
                             uint32_t thrdSpaceDepth,
                             uint32_t grpSpaceWidth,
                             uint32_t grpSpaceHeight,
                             uint32_t grpSpaceDepth,
                             CmThreadGroupSpace* &threadGroupSpace) = 0;

    //!
    //! \brief      Creates a CmSampler8x8 surface by using given 2D surface
    //!             and given flags.
    //! \details    The function indicates the 2D surface is used for sampler
    //!             8x8; no extra surface is actually created. A SurfaceIndex
    //!             object is created instead, which is passed to CM kernel
    //!             function(genx_main) as argument to indicate the surface for
    //!             sampler 8x8. Compared to CmDeive::CreateSampler8x8Surface,
    //!             this API is used to support rotation and chroma siting for
    //!             MediaSampler.
    //! \param      [in] surface2d
    //!             Pointer to CmSurface2D.
    //! \param      [out] sampler8x8SurfIndex
    //!             Reference to pointer to SurfaceIndex.
    //! \param      [in] surfType
    //!             Enumeration data type of CM_SAMPLER8x8_SURFACE.
    //! \param      [in] addressControl
    //!             Enumeration data type of CM_SURFACE_ADDRESS_CONTROL_MODE.
    //! \param      [in] flag
    //!             Pointer to CM_FLAG.
    //! \retval     CM_SUCCESS if the CmSampler8x8 surface is successfully
    //!             created.
    //! \retval     CM_EXCEED_SURFACE_AMOUNT if there is too many co-existed
    //!             surfaces and exceed the maximum number. Destroying some
    //!             unused surfaces could solve this error.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of host memory
    //! \retval     CM_FAILURE otherwise.
    //! \note       This API is not supported in emulation mode.
    //!
    CM_RT_API virtual int32_t
    CreateSampler8x8SurfaceEx(CmSurface2D *surface2d,
                              SurfaceIndex* &sampler8x8SurfIndex,
                              CM_SAMPLER8x8_SURFACE surfType = CM_VA_SURFACE,
                              CM_SURFACE_ADDRESS_CONTROL_MODE addressControl
                                  = CM_SURFACE_CLAMP,
                              CM_FLAG *flag = nullptr) = 0;

    //!
    //! \brief      Creates sampler surface by using given 2D surface and flags.
    //! \details    This sampler surface does't create any actual surface, and
    //!             just bind the actual 2D surface with a virtual sampler
    //!             surface index. User need pass this surface index as kernel
    //!             argument if the surface is used for sampler, otherwise,
    //!             the runtime will report error if user pass the 2D surface
    //!             index. Compared to CmDevice::CreateSampler8x8Surface, this
    //!             API is used to support rotation for 3D sampler. For given
    //!             surface's formats, for now, we support following: \n
    //!                 CM_SURFACE_FORMAT_A16B16G16R16 \n
    //!                 CM_SURFACE_FORMAT_A16B16G16R16F \n
    //!                 CM_SURFACE_FORMAT_R32G32B32A32F \n
    //!                 CM_SURFACE_FORMAT_A8 \n
    //!                 CM_SURFACE_FORMAT_A8R8G8B8 \n
    //!                 CM_SURFACE_FORMAT_YUY2 \n
    //!                 CM_SURFACE_FORMAT_R32F \n
    //!                 CM_SURFACE_FORMAT_R32_UINT \n
    //!                 CM_SURFACE_FORMAT_L16 \n
    //!                 CM_SURFACE_FORMAT_R16G16_UNORM \n
    //!                 CM_SURFACE_FORMAT_R16_FLOAT \n
    //!                 CM_SURFACE_FORMAT_NV12 \n
    //!                 CM_SURFACE_FORMAT_L8 \n
    //!                 CM_SURFACE_FORMAT_AYUV \n
    //!                 CM_SURFACE_FORMAT_Y410 \n
    //!                 CM_SURFACE_FORMAT_Y416 \n
    //!                 CM_SURFACE_FORMAT_Y210 \n
    //!                 CM_SURFACE_FORMAT_Y216 \n
    //!                 CM_SURFACE_FORMAT_P010 \n
    //!                 CM_SURFACE_FORMAT_P016 \n
    //!                 CM_SURFACE_FORMAT_YV12 \n
    //!                 CM_SURFACE_FORMAT_411P \n
    //!                 CM_SURFACE_FORMAT_411R \n
    //!                 CM_SURFACE_FORMAT_IMC3 \n
    //!                 CM_SURFACE_FORMAT_I420 \n
    //!                 CM_SURFACE_FORMAT_422H \n
    //!                 CM_SURFACE_FORMAT_422V \n
    //!                 CM_SURFACE_FORMAT_444P \n
    //!                 CM_SURFACE_FORMAT_RGBP \n
    //!                 CM_SURFACE_FORMAT_BGRP \n
    //! \param      [in] surface2d
    //!             Pointer to CmSurface2D object.
    //! \param      [out] samplerSurfaceIndex
    //!             Reference to the pointer to SurfaceIndex object to be
    //!             created.
    //! \param      [in] flag
    //!             Pointer to CM_FLAG.
    //! \retval     CM_SUCCESS if the new sampler surface index is
    //!             successfully created.
    //! \retval     CM_SURFACE_FORMAT_NOT_SUPPORTED if the input surface format
    //!             is not list above.
    //! \retval     CM_EXCEED_SURFACE_AMOUNT if the total number of created
    //!             co-existed surfaces are exceed maximum count.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of host memory.
    //! \retval     CM_FAILURE otherwise.
    //! \note       This API is not supported in emulation mode.
    //!
    CM_RT_API virtual int32_t
    CreateSamplerSurface2DEx(CmSurface2D *surface2d,
                             SurfaceIndex* &samplerSurfaceIndex,
                             CM_FLAG *flag = nullptr) = 0;

    //!
    //! \brief      Creates an alias to CmBuffer.
    //! \details    Returns a new surface index for this surface. This API is
    //!             used with CmBuffer::SetSurfaceStateParam in order
    //!             to reinterpret buffer for different surface states,
    //!             i.e., the same memory is used but different size
    //!             can be programmed through the surface state.
    //! \param      [in] buffer
    //!             pointer to CmBuffer object used to create an alias.
    //! \param      [out] aliasIndex
    //! \retval     CM_SUCCESS if alias is created successfully.
    //!             new surface index pointing to CmBuffer.
    //! \retval     CM_EXCEED_MAX_NUM_BUFFER_ALIASES if try to create more
    //!             than 10 aliases for same surface.
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of host memory.
    //! \retval     CM_FAILURE if alias cannot be created.
    //! \note       This API is not implemented for EMU mode.
    //!
    CM_RT_API virtual int32_t
    CreateBufferAlias(CmBuffer *buffer,
                      SurfaceIndex* &aliasIndex) = 0;

    //!
    //! \brief      Sets the width and height values in the VME surface state.
    //! \param      [in] vmeIndex
    //!             Pointer to VME surface index.
    //! \param      [in] surfStateParam
    //!             Pointer to CM_VME_SURFACE_STATE_PARAM to set width and
    //!             height of this surface
    //! \retval     CM_SUCCESS if setting VME surface state values successfully.
    //! \retval     CM_INVALID_ARG_VALUE if invalid input.
    //! \note       This API will work on HW and SIM modes.
    //!
    CM_RT_API virtual int32_t
    SetVmeSurfaceStateParam(SurfaceIndex *vmeIndex,
                            CM_VME_SURFACE_STATE_PARAM *surfStateParam) = 0;

    //!
    //! \brief      Gets the VISA version up-to which IGC supports.
    //! \param      [out] majorVersion
    //!             The major Version of VISA.
    //! \param      [out] minorVersion
    //!             The minor Version of VISA.
    //! \retval     CM_SUCCESS if get the right VISA version.
    //! \retval     CM_JITDLL_LOAD_FAILURE if loading igc library is failed.
    //! \retval     CM_FAILURE otherwise.
    //! \note       This API is implemented in hardware mode only.
    //!
    CM_RT_API virtual int32_t GetVISAVersion(uint32_t &majorVersion,
                                             uint32_t &minorVersion) = 0;

    //!
    //! \brief      Creates a CmQueue object with option.
    //! \param      [out] queue
    //!             Pointer to the CmQueue object created.
    //! \param      [in] QueueCreateOption
    //!             The option to create a queue. The sturcture of the
    //!             <b>QueueCreateOption</b> is:\n
    //!             \code
    //!             struct CM_QUEUE_CREATE_OPTION
    //!             {
    //!                 CM_QUEUE_TYPE QueueType : 3;
    //!                 bool RA       : 1;
    //!                 unsigned int Reserved0  : 3;
    //!                 bool UserGPUContext     : 1;
    //!                 unsigned int GPUContext : 8;
    //!                 unsigned int Reserved2  : 16;
    //!             }
    //!             \endcode
    //!             \n
    //!             <b>CM_QUEUE_TYPE</b> indicates which engine the queue will
    //!             be created for:\n
    //!             \code
    //!             enum CM_QUEUE_TYPE
    //!             {
    //!                 CM_QUEUE_TYPE_NONE      = 0,
    //!                 CM_QUEUE_TYPE_RENDER    = 1,
    //!                 CM_QUEUE_TYPE_COMPUTE   = 2,
    //!                 CM_QUEUE_TYPE_VEBOX     = 3
    //!             };
    //!             \endcode
    //!             \n
    //!             <b>RAMode</b> decides if the queue will occupy GPU
    //!             exclusively during execution.\n
    //!             <b>UserGPUContext</b> indicates if the user wants to
    //!             provide an existing MOS GPU Context.\n
    //!             <b>GPUContext</b> is the existing MOS GPU Context Enum
    //!             value.
    //!             \n
    //!             <b>CM_QUEUE_SSEU_USAGE_HINT_TYPE</b> indicates SSEU setting, will
    //!             be created for:\n
    //!             \code
    //!             enum CM_QUEUE_SSEU_USAGE_HINT_TYPE
    //!             {
    //!                 CM_QUEUE_SSEU_USAGE_HINT_DEFAULT = 0,
    //!                 CM_QUEUE_SSEU_USAGE_HINT_VME     = 1
    //!             };
    //!             \endcode
    //! \retval     CM_SUCCESS if the CmQueue object is created.
    //! \note       This API is implemented in hardware mode only. Only
    //!             CM_QUEUE_TYPE_RENDER and CM_QUEUE_TYPE_COMPUTE are
    //!             implemented at this moment.
    //!
    CM_RT_API virtual int32_t
    CreateQueueEx(CmQueue *&queue,
                  CM_QUEUE_CREATE_OPTION QueueCreateOption
                      = CM_DEFAULT_QUEUE_CREATE_OPTION) = 0;

    //!
    //! \brief    Update the MOS Resource in the CmBuffer. If surface is null, 
    //!            creates a new CmBuffer
    //! \details  CmBuffer is a wrapper of that MOS resource. This Mos resource is
    //!            owned by caller.
    //! \param    [in] mosResource
    //!           pointer to MOS resource.
    //! \param    [in,out] surface
    //!           reference to pointer of surface to be created.
    //! \param    [in] mosUsage
    //!           The selected pre-defined MOS usage of memory object control cache setting
    //! \retval   CM_SUCCESS if the CmBuffer is successfully created.
    //! \retval   CM_INVALID_MOS_RESOURCE_HANDLE if mosResource is nullptr.
    //! \retval   CM_OUT_OF_HOST_MEMORY if out of system memory
    //! \retval   CM_EXCEED_SURFACE_AMOUNT if maximum amount of 1D surfaces is exceeded.
    //! \retval   CM_FAILURE otherwise
    //!
    CM_RT_API virtual int32_t UpdateBuffer(PMOS_RESOURCE mosResource, CmBuffer* &surface,
                                           MOS_HW_RESOURCE_DEF mosUsage = MOS_CM_RESOURCE_USAGE_SurfaceState) = 0;

    //!
    //! \brief    Update the MOS Resource in the CmSurface2D. If surface is null, 
    //!            creates a new CmSurface2D
    //! \details  CmSurface2D is a wrapper of that MOS resource. This Mos resource is
    //!            owned by caller.
    //! \param    [in] mosResource
    //!           pointer to MOS resource.
    //! \param    [in,out] surface
    //!           reference to pointer of surface to be created.
    //! \param    [in] mosUsage
    //!           The selected pre-defined MOS usage of memory object control cache setting
    //! \retval   CM_SUCCESS if the CmSurface2D is successfully created.
    //! \retval   CM_INVALID_MOS_RESOURCE_HANDLE if pMosResrouce is nullptr.
    //! \retval   CM_EXCEED_SURFACE_AMOUNT if maximum amount of 2D surfaces
    //!           is exceeded.
    //! \retval   CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t UpdateSurface2D(PMOS_RESOURCE mosResource, CmSurface2D* &surface,
                                              MOS_HW_RESOURCE_DEF mosUsage = MOS_CM_RESOURCE_USAGE_SurfaceState) = 0;

    //!
    //! \brief      Creates a CmSampler8x8 surface from Surface2D alias.
    //! \details    Creates a CmSampler8x8 surface by using the given 2D surface
    //!             alias.
    //!             No extra surface is actually created. A SurfaceIndex
    //!             object is created instead, which is passed to CM kernel
    //!             function (genx_main) as an argument to indicate the surface
    //!             for AVS.
    //! \param      [in] originalSurface
    //!             Pointer to the original CmSurface2D object.
    //! \param      [in] aliasIndex
    //!             Surface alias upon which the output surface index is
    //!             created.
    //! \param      [in] addressControl
    //!             Enumerator specifying address control mode used by AVS.
    //! \param      [out] sampler8x8SurfaceIndex
    //!             Sampler8x8 surface index created by this function.
    //! \retval     CM_SUCCESS if the CmSampler8x8 surface is successfully
    //!             created.
    //! \retval     CM_EXCEED_SURFACE_AMOUNT if there are too many surfaces,
    //!             exceeding the maximum limit,
    //! \retval     CM_OUT_OF_HOST_MEMORY if out of host memory,
    //! \retval     CM_FAILURE otherwise.
    //!
    CM_RT_API virtual int32_t
    CreateSampler8x8SurfaceFromAlias(
        CmSurface2D *originalSurface,
        SurfaceIndex *aliasIndex,
        CM_SURFACE_ADDRESS_CONTROL_MODE addressControl,
        SurfaceIndex* &sampler8x8SurfaceIndex) = 0;
};
}; //namespace

#endif  // #ifndef MEDIADRIVER_LINUX_COMMON_CM_CMDEVICE_H_
