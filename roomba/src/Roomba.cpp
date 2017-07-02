//
//  Roomba.cpp
//  Roomba
//
//  Created by janakiraman gopinath on 2/27/15.
//  Copyright (c) 2015 org.koneksahealth.com. All rights reserved.
//

#include "../hdr/Roomba.h"
#include "../hdr/common.h"

using namespace std;

void Roomba::updatePos()
{
	char str[17];
	float dd = 0, da = 0;

	float speed = (speedR + speedL) / 2;

	conv_to_bin(getSensorData(19), str);
	dd = binsign_to_int(str) + speed*0.02/2.35;

	//printf("%f\n", dd);

	conv_to_bin(getSensorData(20), str);
	da = binsign_to_int(str) + 0.2*atan2(abs((int) speedL) - abs((int) speedR), 23.5);
	//da = binsign_to_int(str) + 0.2*asin(2 * speed /23.5);
	
	angle += (da / PI) * 18.f;
	x += dd*cos( angle * PI / 180.f);
	y += dd*sin( angle * PI / 180.f);
}

void Roomba::printCommands()
{
	for (auto keyValue : this->cmds)
	{
		printf("command[%s]=%d\n", keyValue.first.c_str(), keyValue.second);
	}
}

void Roomba::initializeCommands()
{
	this->cmds["START"] = 128;
	this->cmds["SAFE"] = 131;
	this->cmds["FULL"] = 132;
	this->cmds["SONG"] = 140;
	this->cmds["PLAY"] = 141;
	this->cmds["DRIVE"] = 137;
	this->cmds["DRIVE_DIRECT"] = 145;
	this->cmds["PASSIVE"] = 128;
	this->cmds["STREAM"] = 148;

	this->eventInfo[0].event = (char *)"BUMP";
	this->eventInfo[0].packetId = 7;
	this->eventInfo[0].packetLength = 1;
	this->eventInfo[0].eventMask = 0x1f;

	this->eventInfo[1].event = (char *)"SONG_SELECTED";
	this->eventInfo[1].packetId = 36;
	this->eventInfo[1].packetLength = 1;
	this->eventInfo[1].eventMask = 0x1f;

	this->eventInfo[2].event = (char *)"SONG_PLAYING";
	this->eventInfo[2].packetId = 37;
	this->eventInfo[2].packetLength = 1;
	this->eventInfo[2].eventMask = 0x1f;

	this->eventInfo[3].event = (char *)"VIRTUAL_WALL";
	this->eventInfo[3].packetId = 13;
	this->eventInfo[3].packetLength = 1;
	this->eventInfo[3].eventMask = 0x1f;

	this->eventInfo[4].event = (char *)"CLIFF_LEFT";
	this->eventInfo[4].packetId = 9;
	this->eventInfo[4].packetLength = 1;
	this->eventInfo[4].eventMask = 0xff;

	this->eventInfo[5].event = (char *)"CLIFF_RIGHT";
	this->eventInfo[5].packetId = 12;
	this->eventInfo[5].packetLength = 1;
	this->eventInfo[5].eventMask = 0xff;

	this->eventInfo[6].event = (char *)"CLIFF_FRONT_LEFT";
	this->eventInfo[6].packetId = 10;
	this->eventInfo[6].packetLength = 1;
	this->eventInfo[6].eventMask = 0xff;

	this->eventInfo[7].event = (char *)"CLIFF_FRONT_RIGHT";
	this->eventInfo[7].packetId = 11;
	this->eventInfo[7].packetLength = 1;
	this->eventInfo[7].eventMask = 0xff;
}

void Roomba::destroyThread()
{
	//Destroy Thread
	if (this->threadRunning)
	{
		this->finishThread = true;
		while (this->finishThread) sleepMilliSecond(200);
			this->threadRunning = false;
	}
}

void Roomba::setEvent(char *events[], int total_events, void (*f)(char *, int))
{
	if (!robotReady()) return;

	if (this->threadRunning)
	{
		printf("Cannot intialize events in the middle of a stream");
		return;
	}

	int packetIds[MAX_PACKETS];
	int packet = 0;
	for (int i = 0; i < total_events; i++)
	{
		int index = 0;
		bool found = false;
		while (strcmp(this->eventInfo[index].event,"")  != 0 && index < MAX_EVENTS )
		{
			if (!strcmp(this->eventInfo[index].event, events[i]))
			{
				found = true;
				break;
			}	
			index++;
		}

		if (found) 
		{
			this->events[events[i]].event = this->eventInfo[index];
			this->events[events[i]].f = f;
		}
		else continue;
		
		printf("Found event . %s\n", events[i]);
		//sendCommand((int) this->events.size());

		packetIds[packet] = this->eventInfo[index].packetId;
		packet++;
		for (auto v : this->events)
		{
			packetIds[packet] = v.second.event.packetId;

			sendCommand(v.second.event.packetId);
		}

	}

	sendCommand(this->cmds["STREAM"]);
	sendCommand(packet);
	for (int i = 0; i < packet; i++)
	{
		sendCommand(packetIds[i]);
	}

	this->threadRunning = true;
	this->th = thread(setEventListener, this);
	this->th.detach();
}

void Roomba::setEvents(int events, void (*f)(char *, int))
{
	char *ev[MAX_EVENTS];
	int i = 0;

	if ((events & BUMP_EVENT) == BUMP_EVENT)
		ev[i++] = (char *) "BUMP";

	if ((events & SONG_PLAYING_EVENT) == SONG_PLAYING_EVENT)
		ev[i++] = (char *) "SONG_PLAYING";

	if ((events & VIRTUAL_WALL_EVENT) == VIRTUAL_WALL_EVENT)
		ev[i++] = (char *) "VIRTUAL_WALL";

	if ((events & CLIFF_LEFT_EVENT) == CLIFF_LEFT_EVENT)
		ev[i++] = (char *) "CLIFF_LEFT";

	if ((events & CLIFF_FRONT_LEFT_EVENT) == CLIFF_FRONT_LEFT_EVENT)
		ev[i++] = (char *) "CLIFF_FRONT_LEFT";

	if ((events & CLIFF_RIGHT_EVENT) == CLIFF_RIGHT_EVENT)
		ev[i++] = (char *) "CLIFF_RIGHT";

	if ((events & CLIFF_FRONT_RIGHT_EVENT) == CLIFF_FRONT_RIGHT_EVENT)
		ev[i++] = (char *) "CLIFF_FRONT_RIGHT";

	setEvent(ev, i, f);
}

void Roomba::cliffEvent(void (*f)(char *, int)) 
{
	int event = CLIFF_LEFT_EVENT | CLIFF_FRONT_LEFT_EVENT | CLIFF_RIGHT_EVENT | CLIFF_FRONT_RIGHT_EVENT;
	this->setEvents(event, f);
}

void Roomba::songPlayingEvent(void (*f)(char *, int))
{
	char *events[MAX_EVENTS];
	events[0] = (char *) "SONG_PLAYING";
	this->setEvent(events, 1,  f);
}

void Roomba::bumpEvent(void (*f)(char *, int)) 
{
	char *events[MAX_EVENTS];
	events[0] = (char *) "BUMP";
	this->setEvent(events, 1,  f);
}

void Roomba::virtualWallEvent(void (*f)(char *, int)) 
{
	char *events[MAX_EVENTS];
	events[0] = (char *) "VIRTUAL_WALL";
	this->setEvent(events, 1,  f);
}

void Roomba::stop() 
{
	if (!robotReady()) return;
	sendCommand(this->cmds["PASSIVE"]);

	destroyThread();
	this->mute = true;
	cleanDigitLED();
	setSpeed(0);
	driveDirect(getSpeed(), getSpeed());
	serialClose (this->fd);
	this->isAwake = false;
}

void Roomba::spin(int direction)
{
	sendCommand("DRIVE");
	sendCommand(255);
	sendCommand(56);
	if (direction > 0) 
	{
		sendCommand(0xff);
		sendCommand(0xff);
	} 
	else
	{
		sendCommand(0x00);
		sendCommand(0x01);
	}
}

void Roomba::drive(int velocity, int angle)
{
	if (!robotReady()) return;

	int speed1 = velocity & 0xff00;
	int speed2 = velocity & 0x00ff;
	int angle1 = angle & 0xff00;
	int angle2 = angle & 0x00ff;
	sendCommand("DRIVE");
	sendCommand(speed1);
	sendCommand(speed2);
	sendCommand(angle1);
	sendCommand(angle2);
}

void Roomba::driveDirect(int rightWheelSpeed, int leftWheelSpeed)
{
	speedR = (float) rightWheelSpeed;
	speedL = (float) leftWheelSpeed;

	if (!robotReady()) return;
	
	if( rightWheelSpeed < 0)
		rightWheelSpeed = 65536 + rightWheelSpeed;
	if( leftWheelSpeed < 0)
		leftWheelSpeed = 65536 + leftWheelSpeed;

	int rV1 = (rightWheelSpeed & 0xff00)>>8;
	int rV2 = rightWheelSpeed & 0x00ff;
	int lV1 = (leftWheelSpeed & 0xff00)>>8;
	int lV2 = leftWheelSpeed & 0x00ff;
	sendCommand("DRIVE_DIRECT");
	sendCommand(rV1);
	sendCommand(rV2);
	sendCommand(lV1);
	sendCommand(lV2);
}


void Roomba::turn(int angle)
{
	if( !robotReady()) return;
	this->mute = true;

	angle %= 360;
	if( angle < 0) angle += 360;
	angle /= 8.f;

	char str[17];
	int da, a = getSensorData(20);
	a = 0;
	int signe = (angle > 0) ? 1 : -1;

	setSpeed(50);
	driveDirect( - signe * getSpeed(), signe * getSpeed());

	while( (a < angle && angle > 0) || ( a > angle && angle < 0))
	{
		conv_to_bin(getSensorData(20), str);

		da = 0;
		for( int i = 1; i < 16; ++i)
			da += (1 << (15-i)) * (str[i] == str[0]);
		if( str[0] == '1')
		{
			da ++;
			da *= -1;
		}
		else
			da %= 32767;

		fprintf(stderr, "%d\n", a);
		a += da;
	}
	setSpeed(0);
	driveDirect(getSpeed(), getSpeed());

	this->mute = false;
}

void Roomba::driveDistance( int dFin, int velocity)
{
	if( !robotReady()) return;
	this->mute = true;

	
	char str[17];
	//Initialisation et remise a zero de la distance parcourue
	int dd, d = getSensorData(19);
	d = 0;

	setSpeed(velocity);
	driveDirect(getSpeed(), getSpeed());

	while( d < dFin)
	{
		if( d > dFin - getSpeed() / 4 && getSpeed() > 100)
		{
			d-= velocity/80;
			setSpeed(getSpeed() - 50);
			driveDirect(getSpeed(), getSpeed());		
		}

		conv_to_bin(getSensorData(19), str);
		
		dd = 0;
		for( int i = 1; i < 16; ++i)
			dd += (1 << (15-i)) * (str[i] == str[0]);
		if( str[0] == '1')
		{
			dd ++;
			dd *= -1;
		}
		else
			dd %= 32767;

		fprintf(stderr, "%d\n", d);
		d += dd;
	}

	setSpeed(0);
	driveDirect(getSpeed(), getSpeed());

	this->mute = false;
}

bool Roomba::getStatus() 
{
	return this->isOpen && this->isAwake;
}

void Roomba::sendCommand(string cmd)
{
	sendCommand(this->cmds[cmd]);
}

void Roomba::sendCommand(string cmd, int value)
{
	sendCommand(this->cmds[cmd]);
	sendCommand(value);
}

void Roomba::sendCommand(int value)
{
	size_t n  = write(this->fd, &value , 1);
	if(n < 1) 
		perror("write");
 
 	//Wait 100 default 200 if BR = 115200
	sleepMilliSecond(100);

	//if( ! this->mute)
	//	printf("-[%d]-\n", value);
}

void Roomba::cleanDigitLED()
{
	setDigitLEDs((char*) "    ");
}

void Roomba::writeLEDs( const char* w)
{
	setDigitLEDs(w);
}

void Roomba::writeLEDs( int n)
{
	char w[4];
	for( int i = 0; i < 4; ++i)
	{
		w[3-i] = n - (n/10)*10 + '0';
		n /= 10;
	}
	setDigitLEDs( (char*) w);
}

void Roomba::writeLEDs( int n1, int n2, int n3, int n4)
{
	this->mute = true;
	sendCommand(164);
	sendCommand(n1);
	sendCommand(n2);
	sendCommand(n3);
	sendCommand(n4);
	this->mute = false;
}

void Roomba::setDigitLEDs( const char * w)
{
	this->mute = true;
	sendCommand(164);
	for( int i = 0; i < 4; ++i)
		sendCommand((int) w[i]);
	this->mute = false;
}

void Roomba::sleepMilliSecond(int ms) 
{
	usleep(ms * 1000);
}

bool Roomba::robotReady()
{
	if (!this->getStatus())
	{
		printf("Robot not ready ... \n");
		return false;
	}
	return true;
}

void Roomba::createSong(int songNumber,  int midiLength, array<int, 32> midiSequence)
{
	if (!robotReady()) return;

	this->mute = true;
	sendCommand("SONG");
	sendCommand(songNumber);
	sendCommand(midiLength);

	for (int i = 0; i < midiLength * 2; i++)
		sendCommand(midiSequence.at(i));

	sleepMilliSecond(100);
	this->mute = false;
}

void Roomba::playSong(int songNumber)
{
	if (!robotReady()) return;

	sendCommand("PLAY");
	sendCommand(songNumber);
}

void Roomba::print(int buffer[], int index, int cmd[], int cmd_index)
{
	printf("[ e(%d)", index);
	for (int i = 0; i < index; i++)
		printf("%d,", buffer[i]);
	printf("]\n");
	printf("[ e(%d)", cmd_index);
	for (int i = 0; i < cmd_index; i++)
		printf("%d,", cmd[i]);
	printf("]\n");
}

void Roomba::resetStreamHead(int buffer[], int *index, int command[], int *cmd_index) 
{
	int t_buffer[BUFFER_LIMIT];
	bool startCopy = false;
	int t_index = 0;
	bool eod = false;
	int tmp[BUFFER_LIMIT];
	int tmp_index = 0;

	memset(tmp, 0x00, sizeof(tmp));

	for (int i = 0; i < *index; i++) 
	{
		if (eod) {
			tmp[tmp_index] = buffer[i];
			tmp_index++;
		}
		else if (buffer[i] == STREAM_HEADER && !startCopy)
		{
			t_buffer[t_index] = buffer[i];
			startCopy = true;
			t_index++;
		}
		else if(buffer[i] == STREAM_HEADER && startCopy)
		{
			// Finish up the Command structure
			*cmd_index = t_index;
			memcpy(command, &t_buffer, sizeof(t_buffer));

			// Copy the rest of the data to tmp_buffer before finally merging into return for data resume
			eod = true;
			tmp[tmp_index] = buffer[i];
			tmp_index++;
		}
		else if(startCopy)
		{
			t_buffer[t_index] = buffer[i];
			t_index++;
		}
	}

	*index = tmp_index;
	*cmd_index = t_index;
	memcpy(command, &t_buffer, sizeof(t_buffer));
	memcpy(buffer, &tmp, sizeof(tmp));

	return;
}

void Roomba::setEventListener(Roomba *r)
{
	if (!r->robotReady()) return;

	r->threadRunning = true;
	fd_set fdset, read_set;
	FD_ZERO (&fdset);
	FD_SET (r->fd, &fdset);
	int ret;

	/*struct timeval tm;
	tm.tv_sec = 10;
	tm.tv_usec = 0;*/

	int buffer[BUFFER_LIMIT];
	int cmd[BUFFER_LIMIT];
	int cmd_index = 0;
	int index  = 0;

	while (true)
	{
		if(r->debug) printf("waitOnEvent() ...... \n");
		if ((ret = select (r->fd + 1, &fdset, 0,  0,  0)) < 0)
		{
			perror ("select");
			return;
		}
		if(ret == 0)
		{
			if(r->debug) printf("Select timeout ...\n");
			continue;
		}

		if(r->debug) printf("event() ......\n");
		read_set = fdset;
		int n;
		unsigned char c;

		if (FD_ISSET(r->fd, &fdset))
		{
			while((n = read(r->fd, &c, 1)) > 0)
			{
				if(r->finishThread)
				{
					printf("Asked to exit now ...\n");
					r->finishThread = false;
					r->th.~thread();
					return;
				}
				if(n == -1)
				{
					if(r->debug) printf("streamPacket(1) ...... \n");	
					r->resetStreamHead(buffer, &index, cmd, &cmd_index);
					
					if(r->debug) r->print(buffer, index, cmd, cmd_index);
					r->streamPacket(cmd, cmd_index);

					break;
				}
				buffer[index] = c;
				
				if (buffer[index] == STREAM_HEADER && index > 0 && n > 0)
				{
					if(r->debug) printf("streamPacket(2) ...... \n");
					r->resetStreamHead(buffer, &index, cmd, &cmd_index);
					if(r->debug) r->print(buffer, index, cmd, cmd_index);
					r->streamPacket(cmd, cmd_index);

					index = 0;
				}
				else
					index++;
	
				if(index >= BUFFER_LIMIT) index = 0;
			}
		}
	}

	return;
}

void Roomba::print(int buffer[], int index, int checksum)
{
	printf("[ ");
	for (int i = 0; i <= index; i++)
		printf("%d,", buffer[i]);

	printf("checksum:%d]\n", checksum);
}

void Roomba::streamPacket(int buffer[], int index)
{
	int checksum = 0;

	for (int i = 0; i < index; i++)
		checksum += buffer[i];

	if(this->debug) print(buffer, index, checksum);
	if (checksum != 256 && checksum != 128) return;
	
	int no_of_packets = buffer[1];
	int packetIndex = 2;

	for (int i = 0; i < no_of_packets;)
	{
		int packetId = buffer[packetIndex + i];

		for(auto v : this->events)
		{
			if (v.second.event.packetId == packetId)
			{
				for (int k = 0; k < v.second.event.packetLength; k++)
				{
					if (v.second.event.eventMask & buffer[i+packetIndex+k])
					{
						if(this->debug) printf("event(1)......\n");
						v.second.f(v.first, buffer[packetIndex + i +  k + 1]);
						if(this->debug) printf("f(%s,%d) ...... \n", v.first, buffer[packetIndex +  i + k + 1]);
					}
					i += v.second.event.packetLength + 1;
				}
			}
		}
	}

	return;
}

void Roomba::setDebug(bool debug)
{
	this->debug = debug;
}

void Roomba::wakeUp( char* mode)
{
	printf("Robot initialized ... \n");
	sendCommand("START");
	sendCommand(mode);

	this->isAwake = true;
}

int Roomba::getSpeed()
{
	return (speedL + speedR)/2;
}

void Roomba::setSpeed( int speed)
{
	//this->speed = (speed >= 500)? 500 : (speed <= 0)? 0 : speed;
	//printf("Speed : %d\n", this->speed);
}

void Roomba::setBaudRate( int r)
{
	int baudcode[12] = {300, 600, 1200, 2400, 4800, 9200, 14400, 19200, 28800, 38400, 57600, 115200};
	for( int i = 0; i < 12; ++i)
	{
		if( r == baudcode[i])
		{
			sendCommand(129);
			sendCommand(i);
			return;
		}
	}
	return;
}

void Roomba::listenIR()
{
	sendCommand(142);
	sendCommand(52);

	while (serialDataAvail( this->fd ) > 0)
	{
		printf (" -> %3d", serialGetchar (this->fd)) ;
		fflush (stdout) ;
	}
}

int Roomba::getBattery()
{
	int battery;
	char MSB = 0, LSB = 0;

	sendCommand(142);
	sendCommand(25);
	while (serialDataAvail( this->fd ) > 0)
	{
		MSB = serialGetchar(this->fd);
		LSB = serialGetchar(this->fd);
		battery = (MSB << 7) | LSB;
		printf ("Battery : %3d", battery) ;
		fflush (stdout) ;
	}

	writeLEDs((int) battery);
	return battery;
}

int Roomba::getSensorData( char sensorID)
{
	if( sensorID > 100)
	{
		switch( sensorID)
		{
			case 104:
				sensorID = 7;
				break;
			case 108:
				sensorID = 14;
				break;
			case 116:
				sensorID = 18;
				break;
			case 122:
				sensorID = 45;
				break;
		}
	}

	char MSB = 0, LSB = 0;
	this->mute = true;
	sendCommand(142);
	sendCommand(sensorID);
	this->mute = false;

	if( is_in_array( sensorID))
	{
		return serialGetchar(this->fd);
	}
	else
	{
		MSB = serialGetchar(this->fd);
		LSB = serialGetchar(this->fd);
		return (MSB << 7) | LSB;
	}
}

Roomba::Roomba()
{
	this->isOpen =			false;
	this->isAwake =			false;
	this->debug =			false;
	this->mute =			false;
	this->threadRunning =	false;
	this->finishThread =	false;

	//this->speed = 0;

	x = 0.f;
	y = 0.f;
	angle = 0.f;
	speedR = 0;
	speedL = 0;

	initializeCommands();
	//printCommands();

	if( (this->fd = serialOpen("/dev/ttyAMA0", 19200)) < 0)
	{
		fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
		return;
	}
	else
	{
		this->isOpen = true;
	}

	if( wiringPiSetup() == -1)
	{
		fprintf(stderr, "Unable to strart wiringpi: %s\n", strerror(errno));
		return;
	}

	this->isOpen = true;
	this->isAwake = true;

	getSensorData(19);
	getSensorData(20);

	wakeUp((char*) "SAFE");
	//this->isAwake = true;
}
