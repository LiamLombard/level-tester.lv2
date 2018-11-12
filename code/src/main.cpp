#include <cmath>
#include <memory>
#include <cstdint>
#include <iostream>
#include <fstream>
#include "lv2.h"

constexpr char URI[] = "http://lv2.liamlombard.me/level-tester";
std::ofstream logger;


/**
   In code, ports are referred to by index.  An enumeration of port indices
   should be defined for readability.
*/
enum class PortIndex : uint8_t
{
	IN  = 0,
	OUT = 1
};

/**
   Every plugin defines a private structure for the plugin instance.  All data
   associated with a plugin instance is stored here, and is available to
   every instance method.  In this simple plugin, only port buffers need to be
   stored, since there is no additional instance data.
*/
typedef struct {
	// Port buffers
	const float* input;
	float*       output;
} Plugin;

/**
   The `instantiate()` function is called by the host to create a new plugin
   instance.  The host passes the plugin descriptor, sample rate, and bundle
   path for plugins that need to load additional resources (e.g. waveforms).
   The features parameter contains host-provided features defined in LV2
   extensions, but this simple plugin does not use any.
   This function is in the ``instantiation'' threading class, so no other
   methods on this instance will be called concurrently with it.
*/
static LV2_Handle instantiate(const LV2_Descriptor* descriptor,
            double rate,
            const char* bundle_path,
            const LV2_Feature* const* features)
{
  Plugin* plugin = static_cast<Plugin*>(calloc(1, sizeof(Plugin)));
	return static_cast<LV2_Handle>(plugin);
}

/**
   The `connect_port()` method is called by the host to connect a particular
   port to a buffer.  The plugin must store the data location, but data may not
   be accessed except in run().
   This method is in the ``audio'' threading class, and is called in the same
   context as run().
*/
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

/**
   The `activate()` method is called by the host to initialise and prepare the
   plugin instance for running.  The plugin must reset all internal state
   except for buffer locations set by `connect_port()`.  Since this plugin has
   no other internal state, this method does nothing.
   This method is in the ``instantiation'' threading class, so no other
   methods on this instance will be called concurrently with it.
*/
static void activate(LV2_Handle instance)
{
  logger.open("log.txt");
}

/**
   The `run()` method is the main process function of the plugin.  It processes
   a block of audio in the audio context.  Since this plugin is
   `lv2:hardRTCapable`, `run()` must be real-time safe, so blocking (e.g. with
   a mutex) or memory allocation are not allowed.
*/
static void run(LV2_Handle instance, uint32_t n_samples)
{
	const Plugin* plugin = static_cast<const Plugin*>(instance);
	const float* const input = plugin->input;
	float* const output = plugin->output;

	for (size_t pos = 0; pos < n_samples; ++pos)
  {
		logger << output[pos] << " -> " << input[pos] << '\n';
	}
}

/**
   The `deactivate()` method is the counterpart to `activate()`, and is called by
   the host after running the plugin.  It indicates that the host will not call
   `run()` again until another call to `activate()` and is mainly useful for more
   advanced plugins with ``live'' characteristics such as those with auxiliary
   processing threads.  As with `activate()`, this plugin has no use for this
   information so this method does nothing.
   This method is in the ``instantiation'' threading class, so no other
   methods on this instance will be called concurrently with it.
*/
static void deactivate(LV2_Handle instance)
{
  logger.close();
}

/**
   Destroy a plugin instance (counterpart to `instantiate()`).
   This method is in the ``instantiation'' threading class, so no other
   methods on this instance will be called concurrently with it.
*/
static void cleanup(LV2_Handle instance)
{
	free(instance);
}

/**
   The `extension_data()` function returns any extension data supported by the
   plugin.  Note that this is not an instance method, but a function on the
   plugin descriptor.  It is usually used by plugins to implement additional
   interfaces.  This plugin does not have any extension data, so this function
   returns NULL.
   This method is in the ``discovery'' threading class, so no other functions
   or methods in this plugin library will be called concurrently with it.
*/
static const void* extension_data(const char* uri)
{
	return nullptr;
}

/**
   Every plugin must define an `LV2_Descriptor`.  It is best to define
   descriptors statically to avoid leaking memory and non-portable shared
   library constructors and destructors to clean up properly.
*/
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

/**
   The `lv2_descriptor()` function is the entry point to the plugin library.  The
   host will load the library and call this function repeatedly with increasing
   indices to find all the plugins defined in the library.  The index is not an
   indentifier, the URI of the returned descriptor is used to determine the
   identify of the plugin.
   This method is in the ``discovery'' threading class, so no other functions
   or methods in this plugin library will be called concurrently with it.
*/
LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
	switch (index)
  {
    case 0:  return &descriptor;
    default: return nullptr;
	}
}