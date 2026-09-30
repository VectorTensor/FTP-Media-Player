#include "lib/config/config.h"


#include "libtorrent/magnet_uri.hpp"
#include "libtorrent/session.hpp"

lt::torrent_handle start_torrent_download(lt::session& session, const std::string& url, const std::string& download_path) {
    lt::add_torrent_params atp = lt::parse_magnet_uri(url);
    const std::string base_name = config::get_env_or("CFG_BASE_DOWNLOAD_PATH", "./");
    const bool is_blank = std::all_of(download_path.begin(), download_path.end(),
    [](unsigned char c) { return std::isspace(c); });
    std::ostringstream oss;
    oss << base_name;
    if (!is_blank) {
        oss << download_path;
    }
    std::cout<<"downloadd to : "<<oss.str()<<std::endl;
    atp.save_path = oss.str();
    atp.flags = atp.flags
              & ~lt::torrent_flags::auto_managed   // don't let libtorrent's queue manage it
              & ~lt::torrent_flags::paused;
    lt::torrent_handle h = session.add_torrent(atp);

    return h;
}
