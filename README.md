## 1. ZMQ (Zero Message Queue)를 이용한 간단한 통신 패턴구현 (C++)


ZMQ란?

ZMQ는 Zero Message Queue 의 약어로, 분산 메시징 플랫폼 구현을 위한 라이브러리이다.     
간단하게는 소켓통신을 쉽고 간편하게 구현하는 것 외에도 소켓을 직접 다루는 것 대비하여 더 좋은 기능들을 특별한 비용 없이 제공해준다는 장점이 있다. 

+ ZMQ의 장점 
  + TCP, IPC, inproc, TIPC, Multicast 같은 다양한 프로토콜 및 연결을 사용하여 메시지를 전달할 수 있음    
  + PUB-SUB, ROUTER-DEALER 와 같은 스마트한 메시징 패턴   
  + 비동기 I/O를 지원     
  + 모든 현대 언어와 플랫폼을 지원    
  + 대규모의 활동적인 오픈소스 커뮤니티 지원     
  + 중앙 집중 방식 및 분산 처리 방식, 소규모 및 대규모 등 거의 모든 아키텍쳐에 적용하여 구성 가능    
  + 완전한 상업적 지원을 동반하는 무료 소프트웨어     


1) Request-Reply

REQ-REP 패턴은 클라이언트/서버 모델이며 가장 기본적인 패턴에 해당한다. 클라이언트는 요청을 보내고 서버는 요청에 응답한다.

<img src = "https://user-images.githubusercontent.com/73388615/149617609-f231f192-2cdb-4088-bf6d-891289d74074.png" width="300" height="300">

2) Publish-Subscribe    

PUB-SUB 패턴은 서버가 클라이언트들에게 정보를 PUSH하는 단방향 데이터 배포 패턴이다.

<img src = "https://user-images.githubusercontent.com/73388615/149617709-a25870a7-b767-439d-b2a9-c5cd7506d8e1.png" width="300" height="300">    

3) PipeLine (Push-Pull)

Pipleline 패턴은 PUSH – PULL 패턴의 소켓이 연결되어 단일 방향으로 메시지를 개별전송한다

<img src = "https://user-images.githubusercontent.com/73388615/149618044-e6d90662-aa7a-495b-9295-20e9a8d7e725.png" width = "300" height="300">


