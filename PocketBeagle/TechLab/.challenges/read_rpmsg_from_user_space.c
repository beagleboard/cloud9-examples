/*
 * Source Modified by Pierrick Rauby <PierrickRauby - pierrick.rauby@gmail.com>
 * Based on the examples distributed by TI
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the
 *	  distribution.
 *
 *	* Neither the name of Texas Instruments Incorporated nor the names of
 *	  its contributors may be used to endorse or promote products derived
 *	  from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
   read_rpmsg_from_user_space.c
   Send a message to /dev/rpmsg_pru30 
   Then read the answer from the PRU0
   Wiring:	PocketBeagle + TechLab
   Setup:	tested with 4.19.94-ti-r42 kernel
	 See:
	 PRU:	pru0
   Usage: gcc rpmsg_from_user_space.c -o  rpmsg_from_user_space 
          ./rpmsg_from_user_space
*/

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/poll.h>

#define MAX_BUFFER_SIZE		512
char readBuf[MAX_BUFFER_SIZE];

#define NUM_MESSAGES		10
#define DEVICE_NAME		"/dev/rpmsg_pru30" //Change from /dev/rpmsg_pru31 for PRU1 


static void delay(int number_of_dixieme_de_seconde);

int main(void)
{
	struct pollfd pollfds[1];
	int i;
	int result = 0;

	/* Open the rpmsg_pru character device file */
	pollfds[0].fd = open(DEVICE_NAME, O_RDWR);

	/*
	 * If the RPMsg channel doesn't exist yet the character device
	 * won't either.
	 * Make sure the PRU firmware is loaded and that the rpmsg_pru
	 * module is inserted.
	 */
	if (pollfds[0].fd < 0) {
		printf("Failed to open %s\n", DEVICE_NAME);
		return -1;
	}

	/* The RPMsg channel exists and the character device is opened */
	printf("Opened %s, sending %d messages\n\n", DEVICE_NAME, NUM_MESSAGES);

	for (i = 0; i < NUM_MESSAGES; i++) {
		/* Send 'hello world!' to the PRU through the RPMsg channel */
		result = write(pollfds[0].fd, "hello PRU!", 10);
		// if (result > 0)
		// 	printf("Message %d: Sent to PRU\n", i);
		delay(5); //half a second
		/* Poll until we receive a message from the PRU and then print it */
		result = read(pollfds[0].fd, readBuf, 13);
		if (result > 0)
			printf("Message %d received from PRU:%s\n\n", i, readBuf);
	}

	/* Received all the messages the example is complete */
	printf("Received %d messages, closing %s\n", NUM_MESSAGES, DEVICE_NAME);

	/* Close the rpmsg_pru character device file */
	close(pollfds[0].fd);

	return 0;
}


// C function showing how to do time delay 
#include <stdio.h> 
// To use time library of C 
#include <time.h> 
  
void delay(int number_of_dixieme_de_seconde) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 100000 * number_of_dixieme_de_seconde; 
  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds) 
        ; 
}
