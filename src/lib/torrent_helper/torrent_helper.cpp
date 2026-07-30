


#include "libtorrent/magnet_uri.hpp"
#include "libtorrent/session.hpp"

void start_torrent_download(lt::session& session, const std::string& url) {

    lt::add_torrent_params atp = lt::parse_magnet_uri(url);

}
