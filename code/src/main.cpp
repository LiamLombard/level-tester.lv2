#include <cmath>
#include <memory>
#include <cstdint>
#include <iostream>
#include <fstream>
#include "lv2.h"

constexpr char URI[] = "https://github.com/LiamLombard/level-tester.lv2";
std::ofstream logger;

enum class PortIndex : uint8_t
{
	IN  = 0,
	OUT = 1
};

typedef struct {
	// Port buffers
	const float* input;
	float*       output;
} Plugin;

static LV2_Handle instantiate(const LV2_Descriptor* descriptor,
            double rate,
            const char* bundle_path,
            const LV2_Feature* const* features)
{
  Plugin* plugin = static_cast<Plugin*>(calloc(1, sizeof(Plugin)));
	return static_cast<LV2_Handle>(plugin);
}

static void connect_port(LV2_Handle instance, uint32_t port, void* data)
{
	Plugin* plugin = static_cast<Plugin*>(instance);

	switch (static_cast<PortIndex>(port)) {
	case PortIndex::IN:
		plugin->input = static_cast<const float*>(data);
		break;
	case PortIndex::OUT:
		plugin->output = static_cast<float*>(data);
		break;
	}
}

static void activate(LV2_Handle instance)
{
  logger.open("log.txt");
}

static void run(LV2_Handle instance, uint32_t n_samples)
{
	const Plugin* plugin = static_cast<const Plugin*>(instance);
	const float* const input = plugin->input;
	float* const output = plugin->output;

	for (size_t pos = 0; pos < n_samples; ++pos)
  {
    output[pos] = input[pos];
		logger << input[pos] << '\n';
	}
}

static void deactivate(LV2_Handle instance)
{
  logger.close();
}

static void cleanup(LV2_Handle instance)
{
	free(instance);
}

static const void* extension_data(const char* uri)
{
	return nullptr;
}

static const LV2_Descriptor descriptor = {
	URI,
	instantiate,
	connect_port,
	activate,
	run,
	deactivate,
	cleanup,
	extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
	switch (index)
  {
    case 0:  return &descriptor;
    default: return nullptr;
	}
}