#define PLUG_NAME "QuadFilteredDelay"
#define PLUG_MFR "no-tv.org"
#define PLUG_VERSION_HEX 0x00010002
#define PLUG_VERSION_STR "1.0.2"
#define PLUG_UNIQUE_ID '3khs'
#define PLUG_MFR_ID 'NOTV'
#define PLUG_URL_STR "https://no-tv.org"
#define PLUG_EMAIL_STR "info@no-tv.org"
#define PLUG_COPYRIGHT_STR "Copyright 2025 no-tv.org"
#define PLUG_CLASS_NAME QuadFilteredDelay

#define BUNDLE_NAME "QuadFilteredDelay"
#define BUNDLE_MFR "no-tv.org"
#define BUNDLE_DOMAIN "org"

#define SHARED_RESOURCES_SUBPATH "QuadFilteredDelay"

#define PLUG_CHANNEL_IO "4-4 5-5 6-6 7-7 8-8"

#define PLUG_LATENCY 0
#define PLUG_TYPE 0
#define PLUG_DOES_MIDI_IN 0
#define PLUG_DOES_MIDI_OUT 0
#define PLUG_DOES_MPE 0
#define PLUG_DOES_STATE_CHUNKS 0
#define PLUG_HAS_UI 1
#define PLUG_WIDTH 300
#define PLUG_HEIGHT 600
#define PLUG_FPS 60
#define PLUG_SHARED_RESOURCES 0
#define PLUG_HOST_RESIZE 0

#define AUV2_ENTRY QuadFilteredDelay_Entry
#define AUV2_ENTRY_STR "QuadFilteredDelay_Entry"
#define AUV2_FACTORY QuadFilteredDelay_Factory
#define AUV2_VIEW_CLASS QuadFilteredDelay_View
#define AUV2_VIEW_CLASS_STR "QuadFilteredDelay_View"

#define AAX_TYPE_IDS 'IEF1', 'IEF2'
#define AAX_TYPE_IDS_AUDIOSUITE 'IEA1', 'IEA2'
#define AAX_PLUG_MFR_STR "Acme"
#define AAX_PLUG_NAME_STR "QuadFilteredDelay\nIPEF"
#define AAX_PLUG_CATEGORY_STR "Effect"
#define AAX_DOES_AUDIOSUITE 1

#define VST3_SUBCATEGORY "Fx|Spatial"

#define CLAP_MANUAL_URL "https://iplug2.github.io/manuals/example_manual.pdf"
#define CLAP_SUPPORT_URL "https://github.com/iPlug2/iPlug2/wiki"
#define CLAP_DESCRIPTION "A simple audio effect for modifying gain"
#define CLAP_FEATURES "audio-effect"//, "utility"

#define APP_NUM_CHANNELS 2
#define APP_N_VECTOR_WAIT 0
#define APP_MULT 1
#define APP_COPY_AUV3 0
#define APP_SIGNAL_VECTOR_SIZE 64

#define ROBOTO_FN "Roboto-Regular.ttf"
