#include "expraw.h"
#include <libraw.h>

Expraw::Expraw(QString rawFile)
    :m_rawFileName((rawFile))
{
    m_rawProcessor = new LibRaw();
}

Expraw::~Expraw()
{
    if (m_rawProcessor)
        delete m_rawProcessor;
    m_rawProcessor = nullptr;
}

const RawInfo &Expraw::get_raw_info() const
{
    return m_rawInfo;
}

int Expraw::decode()
{
    int ret = m_rawProcessor->open_file(m_rawFileName.toStdString().c_str());
    if (ret != LIBRAW_SUCCESS)
        return ret;

    if ((ret = m_rawProcessor->unpack()) != LIBRAW_SUCCESS)
    {
        return ret;
    }

    int i=0;

    m_rawInfo.software          = m_rawProcessor->imgdata.idata.software;
    m_rawInfo.normalized_model  = m_rawProcessor->imgdata.idata.normalized_model;
    m_rawInfo.normalized_make   = m_rawProcessor->imgdata.idata.normalized_make;
    m_rawInfo.make              = m_rawProcessor->imgdata.idata.make;
    m_rawInfo.model             = m_rawProcessor->imgdata.idata.model;
    m_rawInfo.dng_version       = m_rawProcessor->imgdata.idata.dng_version;
    m_rawInfo.is_foveon         = m_rawProcessor->imgdata.idata.is_foveon;

    m_rawInfo.raw_width         = m_rawProcessor->imgdata.sizes.raw_width;
    m_rawInfo.raw_height        = m_rawProcessor->imgdata.sizes.raw_height;
    m_rawInfo.top_margin        = m_rawProcessor->imgdata.sizes.top_margin;
    m_rawInfo.left_margin       = m_rawProcessor->imgdata.sizes.left_margin;
    m_rawInfo.flip              = m_rawProcessor->imgdata.sizes.flip;
    m_rawInfo.iwidth            = m_rawProcessor->imgdata.sizes.iwidth;
    m_rawInfo.iheight           = m_rawProcessor->imgdata.sizes.iheight;
    m_rawInfo.width             = m_rawProcessor->imgdata.sizes.width;
    m_rawInfo.height            = m_rawProcessor->imgdata.sizes.height;
    m_rawInfo.raw_pitch         = m_rawProcessor->imgdata.sizes.raw_pitch;

    for(i=0;i<4;i++)
    {
        m_rawInfo.cblack[i]          = m_rawProcessor->imgdata.color.cblack[i];
        m_rawInfo.cam_multiplier[i]  = m_rawProcessor->imgdata.color.cam_mul[i];
        m_rawInfo.pre_multiplier[i]  = m_rawProcessor->imgdata.color.pre_mul[i];
    }

    m_rawInfo.colors             = m_rawProcessor->imgdata.idata.colors;
    m_rawInfo.filters           = m_rawProcessor->imgdata.idata.filters;

    m_rawInfo.black             = m_rawProcessor->imgdata.color.black;
    m_rawInfo.data_maximum      = m_rawProcessor->imgdata.color.data_maximum;
    m_rawInfo.maximum           = m_rawProcessor->imgdata.color.maximum;
    m_rawInfo.fmaximum          = m_rawProcessor->imgdata.color.fmaximum;
    m_rawInfo.raw_bps           = m_rawProcessor->imgdata.color.raw_bps;

    m_rawInfo.bayerFormat = QString("%1%2%3%4")
            .arg(m_rawProcessor->imgdata.idata.cdesc[0])
            .arg(m_rawProcessor->imgdata.idata.cdesc[1])
            .arg(m_rawProcessor->imgdata.idata.cdesc[2])
            .arg(m_rawProcessor->imgdata.idata.cdesc[3]);

    memcpy(&m_rawInfo.linear_max,
           &m_rawProcessor->imgdata.color.linear_max,
           sizeof m_rawProcessor->imgdata.color.linear_max);

    memcpy(&m_rawInfo.black_stat,
           &m_rawProcessor->imgdata.color.black_stat,
           sizeof m_rawProcessor->imgdata.color.black_stat);

    memcpy(&m_rawInfo.WB_Coeffs,
           &m_rawProcessor->imgdata.color.WB_Coeffs,
           sizeof m_rawProcessor->imgdata.color.WB_Coeffs);

    memcpy(&m_rawInfo.WBCT_Coeffs,
           &m_rawProcessor->imgdata.color.WBCT_Coeffs,
           sizeof m_rawProcessor->imgdata.color.WBCT_Coeffs);

    memcpy(&m_rawInfo.cam_xyz,
           &m_rawProcessor->imgdata.color.cam_xyz,
           sizeof m_rawProcessor->imgdata.color.cam_xyz);

    memcpy(&m_rawInfo.rgb_cam,
           &m_rawProcessor->imgdata.color.rgb_cam,
           sizeof m_rawProcessor->imgdata.color.rgb_cam);

    memcpy(&m_rawInfo.cmatrix,
           &m_rawProcessor->imgdata.color.cmatrix,
           sizeof m_rawProcessor->imgdata.color.cmatrix);

    memcpy(&m_rawInfo.dng_color,
            &m_rawProcessor->imgdata.color.dng_color,
            sizeof m_rawProcessor->imgdata.color.dng_color);

    memcpy(&m_rawInfo.dng_level,
            &m_rawProcessor->imgdata.color.dng_levels,
            sizeof m_rawProcessor->imgdata.color.dng_levels);


    m_rawInfo.isBayerPattern    = true;
    if (!(m_rawProcessor->imgdata.idata.filters || m_rawProcessor->imgdata.idata.colors == 1))
    {
        m_rawInfo.isBayerPattern = false;
    }

    if (!m_rawInfo.isBayerPattern)
    {
        if (m_rawProcessor->imgdata.rawdata.color3_image)
        {
            m_rawData = cv::Mat(cv::Size(m_rawProcessor->imgdata.sizes.raw_width, m_rawProcessor->imgdata.sizes.raw_height),
                                CV_16UC3,
                                m_rawProcessor->imgdata.rawdata.color3_image,
                                cv::Mat::AUTO_STEP);
        }
        else if (m_rawProcessor->imgdata.rawdata.color4_image)
        {
            m_rawData = cv::Mat(cv::Size(m_rawProcessor->imgdata.sizes.raw_width, m_rawProcessor->imgdata.sizes.raw_height),
                                CV_16UC4,
                                m_rawProcessor->imgdata.rawdata.color4_image,
                                cv::Mat::AUTO_STEP);
        }
    }
    else
    {
        if(m_rawProcessor->imgdata.rawdata.raw_image)
        {
            m_rawData = cv::Mat(m_rawProcessor->imgdata.sizes.raw_height,
                                m_rawProcessor->imgdata.sizes.raw_width,
                                CV_16UC1,
                                m_rawProcessor->imgdata.rawdata.raw_image);
        }
    }

    return LIBRAW_SUCCESS;
}

const cv::Mat &Expraw::get_raw_data() const
{
    return m_rawData;
}

const char *Expraw::error_message(int e)
{
    return libraw_strerror(e);
}
