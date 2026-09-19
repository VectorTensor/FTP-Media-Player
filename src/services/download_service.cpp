
#include "download_service.h"

#include "libtorrent/session.hpp"
#include <libtorrent/torrent_handle.hpp>

#include "lib/torrent_helper/torrent_helper.h"

DownloadServiceImpl::DownloadServiceImpl(lt::session& session) : session_(session){
}
using torrent_state = libtorrent::torrent_status::state_t;
std::string toStringState(torrent_state t_state) {
    switch (t_state) {
    case torrent_state::downloading:   return "downloading";
    case torrent_state::downloading_metadata : return "downloading_metadata";
    case torrent_state::finished :  return "finished";
    case torrent_state::seeding :  return "seeding";
    default:           return "Unknown";
    }
}grpc::Status DownloadServiceImpl::DownloadAnime(
    grpc::ServerContext* ctx,
    const download::DownloadAnimeRequest *request,
    download::DownloadAnimeResponse* response) {

    // char const* url = "magnet:?xt=urn:btih:7bcc038eee0e2ea139f05b2211e38194d98f213d&dn=%5BDoomdos%5D%20-%20%EB%9D%BC%EC%9D%B4%EC%96%B4%20%EA%B2%8C%EC%9E%84%20-%2021%20%5B1080P%20LFTLNET%20WEB-DL%5D&tr=http%3A%2F%2Fnyaa.tracker.wf%3A7777%2Fannounce&tr=udp%3A%2F%2Fopen.stealth.si%3A80%2Fannounce&tr=udp%3A%2F%2Ftracker.opentrackr.org%3A1337%2Fannounce&tr=udp%3A%2F%2Fexodus.desync.com%3A6969%2Fannounce&tr=udp%3A%2F%2Ftracker.torrent.eu.org%3A451%2Fannounce";
    char const* url = request->url().c_str();
    lt::torrent_handle h = start_torrent_download(session_, url);

    std::string id = InfoHashToString(h.info_hashes());

    {
        std::lock_guard<std::mutex> lock(mutex_);
        handles_[id] = h;
    }
    response->set_message("successfully added torrent");
    response->set_status(true);
    response->set_id(id);
    return grpc::Status::OK;
}

std::string DownloadServiceImpl::InfoHashToString(const lt::info_hash_t& ih)
{
    // v1 hash if present, else v2 — adjust to your libtorrent version's API
    lt::sha1_hash h = ih.get_best();
    std::ostringstream oss;
    for (unsigned char b : h)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)b;
    return oss.str();
}

grpc::Status DownloadServiceImpl::GetStatus(grpc::ServerContext* context, const download::GetStatusRequest* request, download::GetStatusResponse* response)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto id = request->id();
    auto it = handles_.find(id);
    if (it == handles_.end() || !it->second.is_valid())
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "Torrent not found");
    auto t_status= it->second.status();
    response->set_state(toStringState(t_status.state));
    response->set_progress(t_status.progress*100);
    response->set_total_downloaded(t_status.total_done /static_cast<float>(1024 * 1024));
    response->set_total_size(t_status.total/static_cast<float>(1024 * 1024));
    return grpc::Status::OK;

}
