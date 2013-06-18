/*

    Bouncin' SP copyright jarills 2013

    This file is part of Bouncin' SP.

    Bouncin' SP is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bouncin' SP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Bouncin' SP.  If not, see <http://www.gnu.org/licenses/>.

*/

#pragma once

#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

#include <boost/signals2.hpp>

#include "PadInfo.h"

class Pattern;

class Song
{
public:

    struct ExportOptions
    {
        ExportOptions() : normalize_(true), loop_count_(1), keep_tails_(true) {}
        bool normalize_;
        boost::filesystem::path path_;
        unsigned loop_count_;
        bool keep_tails_;
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

    boost::signals2::signal< void( unsigned ) > sig_track_exported_;

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
