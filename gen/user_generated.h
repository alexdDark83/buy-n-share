// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_USER_BS_H_
#define FLATBUFFERS_GENERATED_USER_BS_H_

#include "flatbuffers/flatbuffers.h"

#include "buynshare_generated.h"

namespace bs {

inline const bs::User *GetUser(const void *buf) {
  return flatbuffers::GetRoot<bs::User>(buf);
}

inline bool VerifyUserBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<bs::User>(nullptr);
}

inline void FinishUserBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<bs::User> root) {
  fbb.Finish(root);
}

}  // namespace bs

#endif  // FLATBUFFERS_GENERATED_USER_BS_H_
