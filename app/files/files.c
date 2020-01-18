/*
 * File Name: files.c
 *
 *
 * BSD 3-Clause License
 * 
 * Copyright (c) 2019, nelsoncole
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
 

#include <io.h>


INTN main() {

	UINT32 p1,p2;

	GW_HAND *gw = CreateWindow(TEXT(" Files "),NULL,50,50,900,600, 
	GW_STYLE(FORE_GROUND(GW_WHITE) | BACK_GROUND(GW_DARKGRAY) | BACK_GROUND_STYLE(GW_BLUE)),GW_FLAG_VISIBLE);


	GW_HAND *file = CreateObject(gw,TEXT("FILE NAME"),GW_HANDLE_FILE,200,20,gw->Area.Width -200,gw->Area.Height - 20, 
	GW_STYLE(FORE_GROUND(GW_WHITE) | BACK_GROUND(GW_GRAY)),GW_FLAG_INVISIBLE);

	VFS *vfs = (VFS*)__malloc(0x10000);

	send_msg(MSG_READ_DIR,(UINT32)vfs,0);

	
	UINTN i = 0;
	


	while(TRUE) {

		switch(read_msg(&p1,&p2)) {


		case MSG_READ_KEY:

		switch(p1) {
		case KEY_UP:

			Send(file,0,--i &GW_SMG_NORMAL_BIT);

			break;
		case KEY_LEFT:

			break;
		case KEY_RIGHT:

			break;
		case KEY_DOWN:

			Send(file,0,++i &GW_SMG_NORMAL_BIT);

			break;
		case KEY_ENTER:

			break;

		default:
			break;

		}

			break;

		case MSG_READ_DIR:
		Send(file,(UINT32)vfs,0 &GW_SMG_NORMAL_BIT);
		Send(file,GW_FLAG_VISIBLE,0 |GW_SMG_FLAG_BIT);

			break;


		default:


			break;



		}


		WindowFocus(gw);

	}

	return 0;

}
