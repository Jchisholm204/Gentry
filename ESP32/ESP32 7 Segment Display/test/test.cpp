#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

int main(){
  struct _data{
    int id;
    float a;
    float b;
  };

  _data data{
    32,
    3.3,
    0.06
  };

  char* buffer = (char*)malloc(sizeof(data));

  memcpy(buffer, (char*)&data, sizeof(data));

  printf("%i, %i, %i\n", sizeof(uint64_t), sizeof(float), sizeof(_data));
  for (int i = 0; i < sizeof(_data); i++)
  {
    printf("%i ", buffer[i]);
  }

  _data *parsed;

  memcpy(parsed, buffer, sizeof(_data));

  printf("\n%i %f %f", parsed->id, parsed->a, parsed->b);
  
}