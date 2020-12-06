#include "expraw.h"

std::string type2str(int type)
{
  std::string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

void DumpMatrix(int rows,int cols,float *m)
{
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            if (col == 0)
                printf ("\t");
            else
                printf (" ");
            printf ("%9.6f", m[row*cols+col]) ; //m[row][col]);
        }
        printf ("\n");
    }
}

void outputRawInfo(const RawInfo& raw_info)
{
    printf("Camera Model is %s\n",raw_info.model.toStdString().c_str());
    printf("Manufactured by %s\n",raw_info.make.toStdString().c_str());

    printf("Image raw size %dx%d\n",raw_info.raw_width,raw_info.raw_height);
    printf("Image size %dx%d\n",raw_info.width,raw_info.height);

    //0 if does not require rotation; 3 if requires 180 - deg rotation; 5 if 90 deg counterclockwise, 6 if 90 deg clockwise).
    printf("Flip (0:0,6:90,3:180,5:-90): %d\n", raw_info.flip);

    printf("Left margin %d, Top margin %d\n",raw_info.left_margin,raw_info.top_margin);
    printf("Bayer Pattern: %s\n", raw_info.bayerFormat.toStdString().c_str());
    printf("Raw bits per sample: %d\n", raw_info.raw_bps);
    printf("Color Information\n");
    printf("colors: %d\n",raw_info.colors);
    printf("is_foveon: %d\n",raw_info.is_foveon);
    printf("cblack level: [%d,%d,%d,%d]\n" ,
           raw_info.cblack[0],
            raw_info.cblack[1],
            raw_info.cblack[2],
            raw_info.cblack[3]);
    printf("black_stat: [%d,%d,%d,%d,%d,%d,%d,%d]\n" ,
           raw_info.black_stat[0], raw_info.black_stat[1], raw_info.black_stat[2],  raw_info.black_stat[3],
            raw_info.black_stat[4], raw_info.black_stat[5], raw_info.black_stat[6],  raw_info.black_stat[7]);

    printf("Black = %d\n", raw_info.black);
    printf("data maximum = %d\n", raw_info.data_maximum);
    printf("maximum = %d\n", raw_info.maximum);
    printf("fmaximum = %f\n", raw_info.fmaximum);
    printf("cam_mul = %f, %f, %f, %f\n", raw_info.cam_multiplier[0], raw_info.cam_multiplier[1], raw_info.cam_multiplier[2], raw_info.cam_multiplier[3]);
    printf("pre_mul = %f, %f, %f, %f\n", raw_info.pre_multiplier[0], raw_info.pre_multiplier[1], raw_info.pre_multiplier[2], raw_info.pre_multiplier[3]);
    printf("linear_max = %ld, %ld, %ld, %ld\n", raw_info.linear_max[0], raw_info.linear_max[1], raw_info.linear_max[2], raw_info.linear_max[3]);

    printf("cmatrix:\n");
    DumpMatrix(3,4,(float*)raw_info.cmatrix[0]);

    printf("rgb_cam:\n");
    DumpMatrix(3,4,(float*)raw_info.rgb_cam[0]);

    printf("cam_xyz:\n");
    DumpMatrix(4,3,(float*)raw_info.cam_xyz[0]);

    for(int i=0;i<2;i++)
    {
        printf("DNG colormatrix %d:\n",i);;
        DumpMatrix(4,3,(float*)raw_info.dng_color[i].colormatrix[0]);
    }

    printf("DNG BaselineExposure:%9.6f\n",raw_info.dng_level.baseline_exposure);
    printf("DNG asShotNeutral:\n");
    printf("\t%9.6f, %9.6f, %9.6f, %9.6f\n",
           raw_info.dng_level.asshotneutral[0],
            raw_info.dng_level.asshotneutral[1],
            raw_info.dng_level.asshotneutral[2],
            raw_info.dng_level.asshotneutral[3]);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s [some_raw_file_pathname]\n",argv[0]);
        return -1;
    }

    QString filename(argv[1]);
    Expraw export_raw(filename);
    int ret = export_raw.decode();
    if (ret == 0)
    {
        //get a cv::Mat object raw data
        cv::Mat bayer = export_raw.get_raw_data();
        std::cout << "Raw data mat type: " << type2str(bayer.type()).c_str() << std::endl;

        //get some info of raw image
        RawInfo raw_info = export_raw.get_raw_info();
        outputRawInfo(raw_info);
    }
    else
    {
        printf("something error: %s\n",export_raw.error_message(ret));
    }
    return ret;
}
