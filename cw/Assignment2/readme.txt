Complete the table below with your results, and then provide your interpretation at the end.

Note that:

- When calculating the parallel speed-up S, use the time output by the code, which corresponds
  to the parallel calculation and does not include initialising the matrix/vectors or
  performing the serial check.

- Take as the serial execution time the time output by the code when run with a single process
  (hence the speed-up for 1 process should be 1.0).


No. Process:                        Mean time (average of 3 runs)           Parallel speed-up, S:
===========                         ============================:           ====================
1																		0.0424941				                        1.00
2																		0.0394685			                          1.07
4																		0.0297831		                            1.42
6                                   0.0270559                               1.57
8																		0.0253558			                          1.67

Architecture that the timing runs were performed on:
Device:
 - Device Type: CPU
 - Device name: Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz
 - Device version: OpenCL 2.1 (Build 0)
 - Driver version: 18.1.0.0920
 - Extensions: cl_khr_icd cl_khr_global_int32_base_atomics cl_khr_global_int32_extended_atomics cl_khr_local_int32_base_atomics cl_khr_local_int32_extended_atomics cl_khr_byte_addressable_store cl_khr_depth_images cl_khr_3d_image_writes cl_intel_exec_by_local_thread cl_khr_spir cl_khr_fp64 cl_khr_image2d_from_buffer cl_intel_vec_len_hint 
 - 2 compute unit(s)
 - Max. group work size = 8192
 - Address width is 64 bits


A brief interpretation of these results (2-3 sentences should be enough):
With two processes, the performance gain is not substantial. However, with four processes, the performance gain is noticeable. 
Nevertheless, as the number of processes increases further, the performance gain becomes increasingly marginal.
The reason is When starting using mutliply processes, the initial job may take a lot of resources, then with the increament of processes, more processes will
help the finish the job. But create new process also need resources, thus the speed-up isn't significantly.
