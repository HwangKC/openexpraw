﻿#ifndef EXPRAW_H
#define EXPRAW_H

#include "expraw_global.h"
#include <QString>
#include <libraw/libraw.h>

#define EXPRAW_LIBRAW_CBLACK_SIZE   (4104)
#define EXPRAW_LIBRAW_SUCCESS       (0)

struct EXPRAWSHARED_EXPORT DNGColor
{
    unsigned int    parsedfields;
    ushort          illuminant;
    float           calibration[4][4];
    float           colormatrix[4][3];
    float           forwardmatrix[3][4];
};

struct EXPRAWSHARED_EXPORT DNGLevel
{
    unsigned int    parsedfields;
    unsigned int    dng_cblack[EXPRAW_LIBRAW_CBLACK_SIZE];
    unsigned int    dng_black;
    float           dng_fcblack[EXPRAW_LIBRAW_CBLACK_SIZE];
    float           dng_fblack;
    unsigned int    dng_whitelevel[4];
    unsigned int    default_crop[4]; /* Origin and size */
    unsigned int    preview_colorspace;
    float           analogbalance[4];
    float           asshotneutral[4];
    float           baseline_exposure;
    float           LinearResponseLimit;
};

struct EXPRAWSHARED_EXPORT RawInfo
{
    bool                    isBayerPattern;

    //color parameters
    ushort                  colors;
    unsigned int            cblack[4];
    unsigned int            black;
    unsigned int            data_maximum;
    unsigned int            maximum;
    float                   fmaximum;
    float                   cam_multiplier[4];
    float                   pre_multiplier[4];
    float                   cmatrix[3][4];
    float                   rgb_cam[3][4];
    float                   cam_xyz[4][3];
    long                    linear_max[4];
    unsigned int            raw_bps;
    float                   WBCT_Coeffs[256][4];
    int                     WB_Coeffs[256][4];
    unsigned int            black_stat[8];
    unsigned int            filters;

    //sizes parameters
    ushort                  top_margin;
    ushort                  left_margin;
    ushort                  raw_width;
    ushort                  raw_height;
    ushort                  width;
    ushort                  height;
    ushort                  iwidth;
    ushort                  iheight;
    int                     flip;
    unsigned int            raw_pitch;

    //others
    QString                 software;
    QString                 normalized_model;
    QString                 model;

    QString                 normalized_make;
    QString                 make;

    unsigned int            dng_version;
    unsigned int            is_foveon;

    QString                 bayerFormat;

    DNGColor                dng_color[2];
    DNGLevel                dng_level;
};

class EXPRAWSHARED_EXPORT Expraw
{
public:
    typedef enum _tagBayerDataType
    {
		BDT_UNKNOWN = 0,
        BDT_16UC1	= 2,
		BDT_16UC3	= 18,
		BDT_16UC4	= 26,
    }BAYER_DATA_TYPE;

public:
    Expraw(QString rawFile);
    ~Expraw();

    const RawInfo& get_raw_info() const;

    int	decode();
#ifdef USE_ADOBE_DNG_SDK
    void set_use_dng_sdk();
#endif
    const ushort* bayer_data(ushort& width,ushort& height,BAYER_DATA_TYPE& bdt) const;

public:
    const char  *error_message(int e);
    bool        is_xtrans();
    bool        is_foveon();
private:
    QString             m_rawFileName;
    RawInfo             m_rawInfo;
    ushort*             m_bayerData;
    bool                m_useDngSdk;
    BAYER_DATA_TYPE     m_bayerDataType;
    ushort              m_outputHeight;
    ushort              m_outputWidth;
    bool                m_isXtrans;
    bool                m_isFoveon;
    LibRaw              m_rawProcessor;
};

#endif // EXPRAW_H
