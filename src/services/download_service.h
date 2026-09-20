#pragma once

#include "Download.grpc.pb.h"
#include <grpcpp/support/status.h>

#include "libtorrent/bt_peer_connection.hpp"
class DownloadServiceImpl final : public download::DownloadService::Service {
    lt::session& session_;
    static std::string InfoHashToString(const lt::info_hash_t& ih);
    mutable std::mutex mutex_;
    std::unordered_map<std::string, lt::torrent_handle> handles_;
public:
    DownloadServiceImpl(lt::session &session);

    grpc::Status DownloadAnime(grpc::ServerContext *context,
                      const download::DownloadAnimeRequest *request,
                      download::DownloadAnimeResponse *reply) override;

    grpc::Status GetStatus(grpc::ServerContext* context, const download::GetStatusRequest* request, download::GetStatusResponse* response) override;
    grpc::Status PauseDownload(grpc::ServerContext* context, const download::PauseDownloadRequest* request, download::PauseDownloadResponse* response) override;
    grpc::Status ResumeDownload(grpc::ServerContext* context, const download::ResumeDownloadRequest* request, download::ResumeDownloadResponse* response) override;

};
