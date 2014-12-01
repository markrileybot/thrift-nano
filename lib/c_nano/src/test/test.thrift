
namespace * tn.package.name

struct structb {
  1: string strprop,
  2: byte v1,
  3: byte v2,
  4: byte v3,
  5: i32 v4,
  6: string v5
}

struct structa {
  1: string strprop,
  2: structb structprop,
  3: list<i32> listprop,
  4: map<i16,i16> mapprop
}
