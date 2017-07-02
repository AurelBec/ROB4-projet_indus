#include "../hdr/common.h"
#include "../hdr/Roomba.h"

unsigned char getch()
{
	static struct termios oldt, newt;
	tcgetattr( STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON);                     
	tcsetattr( STDIN_FILENO, TCSANOW, &newt); 

	unsigned char c = getchar(); 

	tcsetattr( STDIN_FILENO, TCSANOW, &oldt); 
	return c;
}

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}

void conv_to_bin(int val, char pStrBin[])
{
	unsigned long nMask = 1;
	unsigned long nIdx = 0;

	for (nIdx = 0; nIdx < 16; nIdx++, nMask <<= 1)
		pStrBin[15 - nIdx] = ((val & nMask) ? '1' : '0');
	pStrBin[16] = '\0';

	return;
}

int binsign_to_int(char pStrBin[])
{
	int res = 0;

	for( int i = 1; i < 16; ++i)
		res += (1 << (15-i)) * (pStrBin[i] != pStrBin[1]);

	if(pStrBin[1] == '0') // cas negatif
		res *= -1;
	
	return res;
}

char single_byte_packets[] = {7, 8, 9, 11, 12, 13, 14, 15, 17, 18, 21, 24, 34, 35, 36, 37, 38, 45, 52, 53, 58};
bool is_in_array(char val)
{
  	for (int i = 0; i < 22; i++)
    		if (val == single_byte_packets[i])
      			return true;

  	return false;
}
