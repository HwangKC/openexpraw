
#include "expraw.h"
#include <libraw/libraw.h>

#ifdef USE_ADOBE_DNG_SDK
#define qWinOS 1
#define qDNGValidateTarget 1
#define qDNGValidate 1
#define qDNGUseLibJPEG  1
#define qDNGXMPFiles 1

#include <dng/dng_host.h>
#endif


Expraw::Expraw(QString rawFile)
    :m_rawFileName((rawFile))
    ,m_useDngSdk(false)
    ,m_bayerData(nullptr)
    ,m_bayerDataType(BDT_UNKNOWN)
    ,m_isXtrans(false)
    ,m_isFoveon(false)
{
}

Expraw::~Expraw()
{
    m_rawProcessor.recycle();
}

const RawInfo &Expraw::get_raw_info() const
{
    return m_rawInfo;
}

#ifdef USE_ADOBE_DNG_SDK
void Expraw::set_use_dng_sdk()
{
    m_useDngSdk = true;
}
#endif

int Expraw::decode()
{
#if USE_ADOBE_DNG_SDK
    if (m_useDngSdk)
    {
        dng_host host;
        m_rawProcessor.set_dng_host(&host);
        m_rawProcessor.imgdata.params.use_dngsdk = LIBRAW_DNG_FLOAT | LIBRAW_DNG_LINEAR | LIBRAW_DNG_XTRANS | LIBRAW_DNG_OTHER;
    }
#endif
    int ret = m_rawProcessor.open_file(m_rawFileName.toStdString().c_str());
    if (ret != LIBRAW_SUCCESS)
        return ret;

    m_rawInfo.software = m_rawProcessor.imgdata.idata.software;
    m_rawInfo.normalized_model = m_rawProcessor.imgdata.idata.normalized_model;
    m_rawInfo.normalized_make = m_rawProcessor.imgdata.idata.normalized_make;
    m_rawInfo.make = m_rawProcessor.imgdata.idata.make;
    m_rawInfo.model = m_rawProcessor.imgdata.idata.model;

    m_isXtrans = false;
    if (m_rawInfo.make.toUpper() == "FUJIFILM")
    {
        //-4 -w -T -v -H 1 -M -o 0
        m_isXtrans = true;
        m_rawProcessor.imgdata.params.output_color = 0; //-o 0
        m_rawProcessor.imgdata.params.highlight = 1; //-H 1
        m_rawProcessor.imgdata.params.use_camera_matrix = 0; //-M
        m_rawProcessor.imgdata.params.gamm[0] = m_rawProcessor.imgdata.params.gamm[1] = m_rawProcessor.imgdata.params.no_auto_bright = 1; //-4
        m_rawProcessor.imgdata.params.use_camera_wb = 1; //-w
        m_rawProcessor.imgdata.params.output_tiff = 1; //-T
        m_rawProcessor.imgdata.params.output_bps = 16; //-4
        m_rawProcessor.imgdata.params.user_qual = 0;
    }

    if ((ret = m_rawProcessor.unpack()) != LIBRAW_SUCCESS)
    {
        return ret;
    }

    int i=0;

    m_rawInfo.raw_width = m_rawProcessor.imgdata.sizes.raw_width;
    m_rawInfo.raw_height = m_rawProcessor.imgdata.sizes.raw_height;
    m_rawInfo.top_margin = m_rawProcessor.imgdata.sizes.top_margin;
    m_rawInfo.left_margin = m_rawProcessor.imgdata.sizes.left_margin;
    m_rawInfo.flip = m_rawProcessor.imgdata.sizes.flip;
    m_rawInfo.iwidth = m_rawProcessor.imgdata.sizes.iwidth;
    m_rawInfo.iheight = m_rawProcessor.imgdata.sizes.iheight;
    m_rawInfo.width = m_rawProcessor.imgdata.sizes.width;
    m_rawInfo.height = m_rawProcessor.imgdata.sizes.height;
    m_rawInfo.raw_pitch = m_rawProcessor.imgdata.sizes.raw_pitch;

    m_rawInfo.dng_version = m_rawProcessor.imgdata.idata.dng_version;
    m_rawInfo.is_foveon = m_rawProcessor.imgdata.idata.is_foveon;

    for(i=0;i<4;i++)
    {
        m_rawInfo.cblack[i]          = m_rawProcessor.imgdata.color.cblack[i];
        m_rawInfo.cam_multiplier[i]  = m_rawProcessor.imgdata.color.cam_mul[i];
        m_rawInfo.pre_multiplier[i]  = m_rawProcessor.imgdata.color.pre_mul[i];
    }

    m_rawInfo.colors             = m_rawProcessor.imgdata.idata.colors;
    m_rawInfo.filters           = m_rawProcessor.imgdata.idata.filters;

    m_rawInfo.black             = m_rawProcessor.imgdata.color.black;
    m_rawInfo.data_maximum      = m_rawProcessor.imgdata.color.data_maximum;
    m_rawInfo.maximum           = m_rawProcessor.imgdata.color.maximum;
    m_rawInfo.fmaximum          = m_rawProcessor.imgdata.color.fmaximum;
    m_rawInfo.raw_bps           = m_rawProcessor.imgdata.color.raw_bps;

    m_rawInfo.bayerFormat = QString("%1%2%3%4")
            .arg(m_rawProcessor.imgdata.idata.cdesc[0])
            .arg(m_rawProcessor.imgdata.idata.cdesc[1])
            .arg(m_rawProcessor.imgdata.idata.cdesc[2])
            .arg(m_rawProcessor.imgdata.idata.cdesc[3]);

    memcpy(&m_rawInfo.linear_max,
           &m_rawProcessor.imgdata.color.linear_max,
           sizeof m_rawProcessor.imgdata.color.linear_max);

    memcpy(&m_rawInfo.black_stat,
           &m_rawProcessor.imgdata.color.black_stat,
           sizeof m_rawProcessor.imgdata.color.black_stat);

    memcpy(&m_rawInfo.WB_Coeffs,
           &m_rawProcessor.imgdata.color.WB_Coeffs,
           sizeof m_rawProcessor.imgdata.color.WB_Coeffs);

    memcpy(&m_rawInfo.WBCT_Coeffs,
           &m_rawProcessor.imgdata.color.WBCT_Coeffs,
           sizeof m_rawProcessor.imgdata.color.WBCT_Coeffs);

    memcpy(&m_rawInfo.cam_xyz,
           &m_rawProcessor.imgdata.color.cam_xyz,
           sizeof m_rawProcessor.imgdata.color.cam_xyz);

    memcpy(&m_rawInfo.rgb_cam,
           &m_rawProcessor.imgdata.color.rgb_cam,
           sizeof m_rawProcessor.imgdata.color.rgb_cam);

    memcpy(&m_rawInfo.cmatrix,
           &m_rawProcessor.imgdata.color.cmatrix,
           sizeof m_rawProcessor.imgdata.color.cmatrix);

    memcpy(&m_rawInfo.dng_color,
            &m_rawProcessor.imgdata.color.dng_color,
            sizeof m_rawProcessor.imgdata.color.dng_color);

    memcpy(&m_rawInfo.dng_level,
            &m_rawProcessor.imgdata.color.dng_levels,
            sizeof m_rawProcessor.imgdata.color.dng_levels);

    m_rawInfo.isBayerPattern    = true;
    if (!(m_rawProcessor.imgdata.idata.filters || m_rawProcessor.imgdata.idata.colors == 1))
    {
        m_rawInfo.isBayerPattern = false;
    }

    m_outputHeight  = m_rawProcessor.imgdata.sizes.raw_height;
    m_outputWidth   = m_rawProcessor.imgdata.sizes.raw_width;

    if (!m_isXtrans)
    {
        if (!m_rawInfo.isBayerPattern)
        {
            if (m_rawProcessor.imgdata.rawdata.color3_image)
            {
                m_bayerDataType = BDT_16UC3;
                m_bayerData = (ushort*)m_rawProcessor.imgdata.rawdata.color3_image;
            }
            else if (m_rawProcessor.imgdata.rawdata.color4_image)
            {
                m_bayerDataType = BDT_16UC4;
                m_bayerData = (ushort*)m_rawProcessor.imgdata.rawdata.color4_image;
            }
            else
            {
                ret = LIBRAW_DATA_ERROR;
            }
        }
        else
        {
            if (m_rawProcessor.imgdata.rawdata.raw_image)
            {
                m_bayerDataType = BDT_16UC1;
                m_bayerData = m_rawProcessor.imgdata.rawdata.raw_image;
            }
            else
            {
                ret = LIBRAW_DATA_ERROR;
            }
        }
    }
    else
    {
        if (LIBRAW_SUCCESS != (ret = m_rawProcessor.dcraw_process()))
        {
            return ret;
        }

        libraw_processed_image_t *p = m_rawProcessor.dcraw_make_mem_image();
        if(p)
        {
            m_bayerDataType = BDT_16UC3;
            m_outputHeight  = p->height;
            m_outputWidth   = p->width;
            m_bayerData     = (ushort*)p->data;
        }
        else
        {
            ret = LIBRAW_DATA_ERROR;
        }
    }

    return ret;
}

const ushort *Expraw::bayer_data(ushort& width,ushort& height,Expraw::BAYER_DATA_TYPE& bdt) const
{
    width = m_outputWidth;
    height = m_outputHeight;
    bdt = m_bayerDataType;
    return m_bayerData;
}

const char *Expraw::error_message(int e)
{
    return libraw_strerror(e);
}

bool Expraw::is_xtrans()
{
    return m_isXtrans;
}

bool Expraw::is_foveon()
{
    return m_isFoveon;
}
