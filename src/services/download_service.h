#pragma once

#include "Download.grpc.pb.h"
#include <grpcpp/support/status.h>

#include "libtorrent/session.hpp"

class DownloadServiceImpl final : public download::DownloadService::Service {
    lt::session& session_;

public:
    DownloadServiceImpl(lt::session &session);

    grpc::Status DownloadAnime(grpc::ServerContext *context,
                      const download::DownloadAnimeRequest *request,
                      download::DownloadAnimeResponse *reply) override;

};
