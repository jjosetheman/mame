// license:GPL-2.0+
// copyright-holders:Peter Trauner
/***************************************************************************

  PeT mess@utanet.at
  main part in video/

***************************************************************************/

#include "emu.h"
#include "vc4000.h"


DEFINE_DEVICE_TYPE(VC4000_SND, vc4000_sound_device, "vc4000_sound", "Interton Electronic VC 4000 Custom Sound")

vc4000_sound_device::vc4000_sound_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
	: device_t(mconfig, VC4000_SND, tag, owner, clock)
	, device_sound_interface(mconfig, *this)
	, m_channel(nullptr)
	, m_reg{ 0 }
	, m_size(0)
	, m_pos(0)
	, m_level(0)
{
}


//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void vc4000_sound_device::device_start()
{
	m_channel = stream_alloc_legacy(0, 1, machine().sample_rate());
}


//-------------------------------------------------
//  sound_stream_update_legacy - handle a stream update
//-------------------------------------------------

void vc4000_sound_device::sound_stream_update_legacy(sound_stream &stream, stream_sample_t const * const *inputs, stream_sample_t * const *outputs, int samples)
{
	int i;
	stream_sample_t *buffer = outputs[0];

	for (i = 0; i < samples; i++, buffer++)
	{
		*buffer = 0;
		if (m_reg[0] && m_pos <= m_size / 2)
		{
			*buffer = 0x7fff;
		}
		if (m_pos <= m_size)
			m_pos++;
		if (m_pos > m_size)
			m_pos = 0;
	}
}


void vc4000_sound_device::soundport_w(int offset, int data)
{
	m_channel->update();
	m_reg[offset] = data;
	switch (offset)
	{
		case 0:
			m_pos = 0;
			m_level = true;
			// frequency 7874/(data+1)
			m_size = machine().sample_rate() * (data + 1) /7874;
			break;
	}
}
