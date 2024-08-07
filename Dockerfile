FROM cg/compile-x86:rust2ndclang12
LABEL authors="LL06p"

RUN apt update
RUN apt install cmake gdb -y

ENTRYPOINT ["bash"]
