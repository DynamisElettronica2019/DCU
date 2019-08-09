#include "string_utility.h"
#include "data.h"


// Funzione per convertire in stringa i numeri che sono nella forma 3 cifre intere + 2 decimali + segno
// Il formato è fisso e non può essere usata per altri tipi di conversioni
// Il punto decimale è direttamente inserio dalla funzione, quindi non scirverlo prima nel buffer
// Gestisce anche l'eventuale segno del numero
extern inline void decimalToString(int16_t i, uint8_t *s, uint8_t numInt, uint8_t nunDec)
{
	int16_t divResult;
	int16_t integer;
	int16_t decimal;
	uint8_t charSizeCount = 0;
	uint8_t negativeZeroFlag = 0;

  divResult = i;

	// Divido l'intero per 10 fino a quando non è nullo, ovvero conto le cifre del numero da covertire
	while(divResult != 0)
	{
		divResult /= 10;
		charSizeCount++;
	}

	if(charSizeCount > (numInt + nunDec))
	{
		for(uint8_t j = 0; j < (numInt + nunDec + 1); j++)
		{
			s[j] = '*';
		}
  }

  else
  {
    integer = i / power(10, nunDec);

    if(i < 0)
    {
      i *= -1;
			if (integer == 0){
				negativeZeroFlag = 1;
			}
    }

    decimal = i % power(10, nunDec);
    intToString((int16_t)integer, &s[0], numInt + 1);
		if(negativeZeroFlag){
			s[0] = '-';
		}
    s[numInt+1] = DECIMAL_SEPARATOR;
    intToString((int16_t)decimal, &s[numInt + 2], nunDec);
  }
}


// Funzione per convertire in stringa i numeri che sono nella forma 3 cifre intere + 2 decimali
// Il formato è fisso e non può essere usata per altri tipi di conversioni
// Il punto decimale è direttamente inserio dalla funzione, quindi non scirverlo prima nel buffer
// La funzione gestisce correttamente soltanto numeri positivi, nel formato indicato
extern inline void decimalToStringUnsigned(uint16_t i, uint8_t *s, uint8_t numInt, uint8_t nunDec)
{
	uint16_t divResult;
	uint16_t integer;
	uint16_t decimal;
	uint8_t charSizeCount = 0;

  divResult = i;

	// Divido l'intero per 10 fino a quando non è nullo, ovvero conto le cifre del numero da covertire
	while(divResult != 0)
	{
		divResult /= 10;
		charSizeCount++;
	}

	if(charSizeCount > (numInt + nunDec))
	{
		for(uint8_t j = 0; j < (numInt + nunDec + 1); j++)
		{
			s[j] = '*';
		}
  }

  else
  {
    integer = i / power(10, nunDec);
    decimal = i % power(10, nunDec);
    intToStringUnsigned((int16_t)integer, &s[0], numInt);
    s[numInt] = DECIMAL_SEPARATOR;
    intToStringUnsigned((int16_t)decimal, &s[numInt + 1], nunDec);
  }
}


// Funzione di conversione da int a stringa di caratteri
// Se il numero richiede un numero di caratteri maggiore di size, scrive '*'
extern inline void intToString(int16_t i, uint8_t *s, uint8_t size)
{
	uint8_t count = 0;
	uint8_t charSizeCount = 0;
	uint8_t j = 0;
	int16_t divResult;

	// Se il numero è negatico, lo trasformo in uno positivo mettendo il meno davanti
	if(i < 0)
  {
		s[0] = '-';
		i = i * (-1);
		charSizeCount = 1;
    j = 1;
	}
  
	divResult = i;

	// Divido l'intero per 10 fino a quando non è nullo, ovvero conto le cifre del numero da covertire
	while((divResult != 0) || (count == 0))
	{
		divResult = divResult / 10;
		count++;
		charSizeCount++;
	}

	divResult = i
  ;

	// Controllo se il nuemro da convertire richiede più spazio di quello voluto (parametro size)
	// Se lo richiede scrivo sempre '*', alrimenti lo converto nella stringa
	if(charSizeCount > size)
  {
		for(j = 0; j < size; j++)
		{
			s[j] = '*';
		}
	}

	else
	{
    
		// Se l'intero richiede un numero di cifre inferiore a quello voluto lo completo con zeri
		while(charSizeCount < size)
		{
			s[j] = '0';
			j++;
			charSizeCount++;
		}

		// Include nella stringa il resto della divisione intera del numero per 10
		// Riduco in numero di un ordine di grandezza e il resto lo includo nell'iterazione successiva
		for(j = 0; j < count; j++)
		{
			s[(charSizeCount - 1) - j] = (divResult % 10) + '0';
			divResult /= 10;
		}
	}
}


extern inline void intToStringUnsigned(uint16_t i, uint8_t *s, uint8_t size)
{
	uint8_t count = 0;
	uint8_t charSizeCount = 0;
	uint8_t j = 0;
	uint16_t divResult;

  divResult = i;

	// Divido l'intero per 10 fino a quando non è nullo, ovvero conto le cifre del numero da covertire
	while((divResult != 0) || (count == 0))
	{
		divResult = divResult / 10;
		count++;
		charSizeCount++;
	}

	divResult = i;

	// Controllo se il nuemro da convertire richiede più spazio di quello voluto (parametro size)
	// Se lo richiede scrivo sempre '*', alrimenti lo converto nella stringa
	if(charSizeCount > size)
  {
		for(j = 0; j < size; j++)
		{
			s[j] = '*';
		}
	}

	else
	{
    
		// Se l'intero richiede un numero di cifre inferiore a quello voluto lo completo con zeri
		while(charSizeCount < size)
		{
			s[j] = '0';
			j++;
			charSizeCount++;
		}

		// Include nella stringa il resto della divisione intera del numero per 10
		// Riduco in numero di un ordine di grandezza e il resto lo includo nell'iterazione successiva
		for(j = 0; j < count; j++)
		{
			s[(charSizeCount - 1) - j] = (divResult % 10) + '0';
			divResult /= 10;
		}
	}
}

extern inline void int32ToString(int32_t i, uint8_t *s, uint8_t size)
{
  uint8_t count = 0;
	uint8_t charSizeCount = 0;
	uint8_t j = 0;
	int32_t divResult;
	
	// Se il numero è negatico, lo trasformo in uno positivo mettendo il meno davanti
	if(i < 0)
  {
		s[0] = '-';
		i = i * (-1);
		charSizeCount = 1;
    j = 1;
	}

	divResult = i;

	// Divido l'intero per 10 fino a quando non è nullo, ovvero conto le cifre del numero da covertire
	while((divResult != 0) || (count == 0))
	{
		divResult = divResult / 10;
		count++;
		charSizeCount++;
	}

	divResult = i;

	// Controllo se il nuemro da convertire richiede più spazio di quello voluto (parametro size)
	// Se lo richiede scrivo sempre '*', alrimenti lo converto nella stringa
	if(charSizeCount > size)
  {
		for(j = 0; j < size; j++)
		{
			s[j] = '*';
		}
	}

	else
	{
    
		// Se l'intero richiede un numero di cifre inferiore a quello voluto lo completo con zeri
		while(charSizeCount < size)
		{
			s[j] = '0';
			j++;
			charSizeCount++;
		}

		// Include nella stringa il resto della divisione intera del numero per 10
		// Riduco in numero di un ordine di grandezza e il resto lo includo nell'iterazione successiva
		for(j = 0; j < count; j++)
		{
			s[(charSizeCount - 1) - j] = (divResult % 10) + '0';
			divResult /= 10;
		}
	}
}

extern inline void uint32ToString(uint32_t i, uint8_t *s, uint8_t size)
{
  uint8_t count = 0;
	uint8_t charSizeCount = 0;
	uint8_t j = 0;
	uint32_t divResult;

	divResult = i;

	// Divido l'intero per 10 fino a quando non è nullo, ovvero conto le cifre del numero da covertire
	while((divResult != 0) || (count == 0))
	{
		divResult = divResult / 10;
		count++;
		charSizeCount++;
	}

	divResult = i;

	// Controllo se il nuemro da convertire richiede più spazio di quello voluto (parametro size)
	// Se lo richiede scrivo sempre '*', alrimenti lo converto nella stringa
	if(charSizeCount > size)
  {
		for(j = 0; j < size; j++)
		{
			s[j] = '*';
		}
	}

	else
	{
    
		// Se l'intero richiede un numero di cifre inferiore a quello voluto lo completo con zeri
		while(charSizeCount < size)
		{
			s[j] = '0';
			j++;
			charSizeCount++;
		}

		// Include nella stringa il resto della divisione intera del numero per 10
		// Riduco in numero di un ordine di grandezza e il resto lo includo nell'iterazione successiva
		for(j = 0; j < count; j++)
		{
			s[(charSizeCount - 1) - j] = (divResult % 10) + '0';
			divResult /= 10;
		}
	}
}

extern inline uint8_t getByteFromString(uint8_t char1, uint8_t char2)
{
  uint8_t byte;
  uint8_t temp;
  
  temp = char1 - '0';
  byte = temp * 10;
  temp = char2 - '0';
  byte = byte + temp;
  return byte;
}


static inline uint16_t power(uint16_t base, uint16_t num)
{
  uint16_t result = base;
  
  for(uint16_t i = 1; i < num; i++)
  {
      result = result*base;
  }
  return result;
}
