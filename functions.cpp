#include <nan.h>
#include "blake2/blake2.h"
#include "xorshift.hpp"
#include <random>
#include <string>
#include <sstream>
#include <iomanip>

void HexToBytes(std::string hex, uint8_t * bytes) {

    int j = 0;
    for (unsigned int i = 0; i < hex.length(); i += 2)
    {
        std::string byteString = hex.substr(i, 2);
        uint8_t byte = (uint8_t) strtol(byteString.c_str(), NULL, 16);
        bytes[j] = byte;
        j++;
    }

}

uint64_t iterations(uint8_t * bytes)
{
    uint64_t threshold = 0xffffffc000000000;
    uint64_t work;
    uint64_t output = 0;
    blake2b_state hash;
    blake2b_init (&hash, sizeof(output));
    std::xorshift1024star rng;

    const int range_from  = 0;
    const int range_to    = 32767;
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(range_from, range_to);

    for(int j = 0; j < 16; j++)
        rng.s[j] = distr(generator);

    while(output < threshold)
    {
		work = rng.next ();
		blake2b_update (&hash, reinterpret_cast <uint8_t *> (&work), sizeof (work));
		blake2b_update (&hash, bytes, 32);
		blake2b_final (&hash, reinterpret_cast <uint8_t *> (&output), sizeof (output));
		blake2b_init (&hash, sizeof (output));
		iteration -= 1;
    }

		return work;
}

void Calculate(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
  if (info.Length() < 1 || !info[0]->IsString()) {
    Nan::ThrowTypeError("Must supply one string argument");
    return;
  }

	std::string hex = std::string(*v8::String::Utf8Value(info[0]->ToString()));
	uint8_t bytes[32];
	int j = 0;
	for (unsigned int i = 0; i < hex.length(); i += 2)
	{
			std::string byteString = hex.substr(i, 2);
			uint8_t byte = (uint8_t) strtol(byteString.c_str(), NULL, 16);
			bytes[j] = byte;
			j++;
	}
	uint64_t work = iterations(bytes);
  std::ostringstream oss;
	oss << std::hex <<  work;
	std::string intAsString(oss.str());
  info.GetReturnValue().Set(Nan::New(intAsString).ToLocalChecked());
}

void Init(v8::Local<v8::Object> exports) {
  exports->Set(Nan::New("calculate").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Calculate)->GetFunction());
}

NODE_MODULE(functions, Init)
