
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

    char const* url = request->url().c_str();
    char const* download_path = request->download_path().c_str();
    lt::torrent_handle h = start_torrent_download(session_, url, download_path);

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
        return {grpc::StatusCode::NOT_FOUND, "Torrent not found"};
    auto t_status= it->second.status();
    response->set_state(toStringState(t_status.state));
    response->set_progress(t_status.progress*100);
    response->set_total_downloaded(t_status.total_done /static_cast<float>(1024 * 1024));
    response->set_total_size(t_status.total/static_cast<float>(1024 * 1024));
    response->set_download_speed(t_status.download_rate/static_cast<float>(1024));
    if (t_status.flags & lt::torrent_flags::paused)
    {
        response->set_ispaused(true);
    }
    else
    {

        response->set_ispaused(false);
    }
    return grpc::Status::OK;

}

grpc::Status DownloadServiceImpl::PauseDownload(grpc::ServerContext* context, const download::PauseDownloadRequest* request, download::PauseDownloadResponse* response)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto id = request->id();
    auto it = handles_.find(id);
    if (it == handles_.end() || !it->second.is_valid())
        return {grpc::StatusCode::NOT_FOUND, "Torrent not found"};
    it->second.pause();
    response->set_status(true);
    response->set_message("successfully paused");

    return grpc::Status::OK;

}

grpc::Status DownloadServiceImpl::ResumeDownload(grpc::ServerContext* context, const download::ResumeDownloadRequest* request, download::ResumeDownloadResponse* response)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto id = request->id();
    auto it = handles_.find(id);
    if (it == handles_.end() || !it->second.is_valid())
        return {grpc::StatusCode::NOT_FOUND, "Torrent not found"};
    it->second.resume();
    response->set_status(true);
    response->set_message("successfully resumed");

    return grpc::Status::OK;
}

grpc::Status DownloadServiceImpl::SetDownloadLimit(grpc::ServerContext* context, const download::SetDownloadRequest* request, download::SetDownloadResponse* response)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto id = request->id();
    auto it = handles_.find(id);
    if (it == handles_.end() || !it->second.is_valid())
        return {grpc::StatusCode::NOT_FOUND, "Torrent not found"};
    int limit = request->limit();
    it->second.set_download_limit(limit * 1024);
    response->set_status(true);
    response->set_message("successfully added limit");

    return grpc::Status::OK;

}

grpc::Status DownloadServiceImpl::UnsetDownloadLimit(grpc::ServerContext* context, const download::UnsetDownloadRequest* request, download::SetDownloadResponse* response)
{

    std::lock_guard<std::mutex> lock(mutex_);
    auto id = request->id();
    auto it = handles_.find(id);
    if (it == handles_.end() || !it->second.is_valid())
        return {grpc::StatusCode::NOT_FOUND, "Torrent not found"};
    it->second.set_download_limit(-1);
    response->set_status(true);
    response->set_message("successfully removed limit");

    return grpc::Status::OK;
}
