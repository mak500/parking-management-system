#ifndef CLIENT_HH
#define CLIENT_HH

#include <memory>

#include "parking_management.grpc.pb.h"
#include "parking_management.pb.h"
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>

namespace client {
class ParkingManagerClient {
private:
  std::unique_ptr<ParkingManager::Stub> m_stub;

public:
  ParkingManagerClient(std::shared_ptr<grpc::ChannelInterface> channel)
      : m_stub(ParkingManager::NewStub(channel)) {}

  /// Creates a parking lot via the user given parameter.
  void createParkingLot();
};
} // namespace client

#endif // CLIENT_HH