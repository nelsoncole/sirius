/*
 * File Name: msgbox.c
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


INTN main(INTN argc,CHAR8 *argv[])
{

	GW_HAND *gw = CreateWindow(TEXT(" MsgBox "),NULL,50,200,220,120, 
	GW_STYLE(FORE_GROUND(GW_WHITE) | BACK_GROUND(GW_GRAY) | BACK_GROUND_STYLE(GW_DARKGRAY)),GW_FLAG_VISIBLE);

	GW_HAND *glabel1 = CreateObject(gw,TEXT("This is a mensagem box!"),GW_HANDLE_LABEL,8,16,200,50, 
	GW_STYLE(FORE_GROUND(GW_WHITE) | BACK_GROUND(GW_GRAY)),GW_FLAG_INVISIBLE);

	GW_HAND *gbutton1 = CreateObject(gw,TEXT("OK"),GW_HANDLE_BUTTON,70,40,60,28, 
	GW_STYLE(FORE_GROUND(GW_WHITE) | BACK_GROUND(GW_DARKGRAY)),GW_FLAG_INVISIBLE);


	// enviar sms para label 1
	Send(glabel1,GW_FLAG_VISIBLE,GW_SMG_FLAG_BIT);
	// enviar sms para button 1
	Send(gbutton1,GW_FLAG_VISIBLE,GW_SMG_FLAG_BIT);
	

	//for(;;);

	gui_exit(gw);

	return 0;
}
