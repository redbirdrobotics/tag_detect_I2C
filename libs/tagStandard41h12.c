/* Copyright (C) 2013-2016, The Regents of The University of Michigan.
All rights reserved.
This software was developed in the APRIL Robotics Lab under the
direction of Edwin Olson, ebolson@umich.edu. This software may be
available under alternative licensing terms; contact the address above.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the Regents of The University of Michigan.
*/

#include <stdlib.h>
#include "tagStandard41h12.h"

static uint64_t codedata[5] = {
   0x000001bd8a64ad10UL,
   0x000001bdc4f3b2d5UL,
   0x000001bdff82b89aUL,
   0x000001be3a11be5fUL,
   0x000001be74a0c424UL
   };
apriltag_family_t *tagStandard41h12_create()
{
   apriltag_family_t *tf = calloc(1, sizeof(apriltag_family_t));
   tf->name = strdup("tagStandard41h12");
   tf->h = 12;
   tf->ncodes = 5;
   tf->codes = codedata;
   tf->nbits = 41;
   tf->bit_x = calloc(41, sizeof(uint32_t));
   tf->bit_y = calloc(41, sizeof(uint32_t));
   tf->bit_x[0] = -2;
   tf->bit_y[0] = -2;
   tf->bit_x[1] = -1;
   tf->bit_y[1] = -2;
   tf->bit_x[2] = 0;
   tf->bit_y[2] = -2;
   tf->bit_x[3] = 1;
   tf->bit_y[3] = -2;
   tf->bit_x[4] = 2;
   tf->bit_y[4] = -2;
   tf->bit_x[5] = 3;
   tf->bit_y[5] = -2;
   tf->bit_x[6] = 4;
   tf->bit_y[6] = -2;
   tf->bit_x[7] = 5;
   tf->bit_y[7] = -2;
   tf->bit_x[8] = 1;
   tf->bit_y[8] = 1;
   tf->bit_x[9] = 2;
   tf->bit_y[9] = 1;
   tf->bit_x[10] = 6;
   tf->bit_y[10] = -2;
   tf->bit_x[11] = 6;
   tf->bit_y[11] = -1;
   tf->bit_x[12] = 6;
   tf->bit_y[12] = 0;
   tf->bit_x[13] = 6;
   tf->bit_y[13] = 1;
   tf->bit_x[14] = 6;
   tf->bit_y[14] = 2;
   tf->bit_x[15] = 6;
   tf->bit_y[15] = 3;
   tf->bit_x[16] = 6;
   tf->bit_y[16] = 4;
   tf->bit_x[17] = 6;
   tf->bit_y[17] = 5;
   tf->bit_x[18] = 3;
   tf->bit_y[18] = 1;
   tf->bit_x[19] = 3;
   tf->bit_y[19] = 2;
   tf->bit_x[20] = 6;
   tf->bit_y[20] = 6;
   tf->bit_x[21] = 5;
   tf->bit_y[21] = 6;
   tf->bit_x[22] = 4;
   tf->bit_y[22] = 6;
   tf->bit_x[23] = 3;
   tf->bit_y[23] = 6;
   tf->bit_x[24] = 2;
   tf->bit_y[24] = 6;
   tf->bit_x[25] = 1;
   tf->bit_y[25] = 6;
   tf->bit_x[26] = 0;
   tf->bit_y[26] = 6;
   tf->bit_x[27] = -1;
   tf->bit_y[27] = 6;
   tf->bit_x[28] = 3;
   tf->bit_y[28] = 3;
   tf->bit_x[29] = 2;
   tf->bit_y[29] = 3;
   tf->bit_x[30] = -2;
   tf->bit_y[30] = 6;
   tf->bit_x[31] = -2;
   tf->bit_y[31] = 5;
   tf->bit_x[32] = -2;
   tf->bit_y[32] = 4;
   tf->bit_x[33] = -2;
   tf->bit_y[33] = 3;
   tf->bit_x[34] = -2;
   tf->bit_y[34] = 2;
   tf->bit_x[35] = -2;
   tf->bit_y[35] = 1;
   tf->bit_x[36] = -2;
   tf->bit_y[36] = 0;
   tf->bit_x[37] = -2;
   tf->bit_y[37] = -1;
   tf->bit_x[38] = 1;
   tf->bit_y[38] = 3;
   tf->bit_x[39] = 1;
   tf->bit_y[39] = 2;
   tf->bit_x[40] = 2;
   tf->bit_y[40] = 2;
   tf->width_at_border = 5;
   tf->total_width = 9;
   tf->reversed_border = true;
   return tf;
}

void tagStandard41h12_destroy(apriltag_family_t *tf)
{
   free(tf->bit_x);
   free(tf->bit_y);
   free(tf->name);
   free(tf);
}
