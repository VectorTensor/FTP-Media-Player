
#include "download_service.h"

#include "libtorrent/session.hpp"


DownloadServiceImpl::DownloadServiceImpl(lt::session& session) : session_(session){
}

grpc::Status DownloadServiceImpl::DownloadAnime(
    grpc::ServerContext* ctx,
    const download::DownloadAnimeRequest *request,
    download::DownloadAnimeResponse* response) {


    return grpc::Status::OK;
}
