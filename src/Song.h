#pragma once

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

#include "PadInfo.h"

class Pattern;

class Song
{
public:

    struct ExportOptions
    {
        ExportOptions() : normalize_(true) {}
        bool normalize_;
        boost::filesystem::path path_;
    };

public:

    Song( const boost::filesystem::path & base_folder );

    std::shared_ptr<Pattern> load_pattern(const boost::filesystem::path &ptn_file, unsigned idx);

    bool export_pattern( const Pattern & ptn, const ExportOptions & export_options );
    bool export_pad_in_pattern(const Pattern & ptn, unsigned pad_idx, const ExportOptions & export_options );

    bool load_patterns();

    std::string pattern_name( unsigned idx ) const;

    const std::vector< std::shared_ptr<Pattern> > & patterns() const;

    float bpm() const;

    const boost::filesystem::path & base_folder() const;

private:

    bool read_stp_info(const boost::filesystem::path & stp_path );

private:

    boost::filesystem::path base_folder_;
    boost::filesystem::path sample_path_;
    boost::filesystem::path pattern_path_;
    std::vector< PadInfo > pad_infos_;
    std::vector< std::shared_ptr<Pattern> > patterns_;
    float bpm_;

};