#pragma once

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

#include "PadInfo.h"

class Pattern;

class Song
{
public:

    Song( const boost::filesystem::path & base_folder );
    bool export_pattern( unsigned ptn_idx );
    bool export_pad_in_pattern(const Pattern & ptn, unsigned pad_idx );

private:

    bool read_stp_info(const boost::filesystem::path & stp_path );

private:

    boost::filesystem::path sample_path_;
    boost::filesystem::path pattern_path_;
    std::vector< PadInfo > pad_infos_;
    float bpm_;

};
