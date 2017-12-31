
#ifndef __PCMCAPTURE_H__
#define __PCMCAPTURE_H__

typedef struct
{
    U8 riffID[4];
    U32 riffSize;
    U8 riffFormat[4];
}t_RIFF;

typedef struct
{
    U16 wFormatTag;
    U16 nChannels;
    U32 nSamplesPerSec;
    U32 nAvgBytesPerSec;
    U16 nBlkAlign;
    U16 wBitsPerSample;
}t_FMTFORMAT;

typedef struct
{
    U8 fmtID[4];
    U32 fmtSize;
    t_FMTFORMAT fmtFormat;
}t_FMT;

typedef struct
{
    U8 dataID[4];
    U32 dataSize;
}t_DATA;

#endif //#ifndef __PCMCAPTURE_H__

