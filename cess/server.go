package main

import (
  "encoding/json"
  "log"
  "fmt"
  "net/http"

  "github.com/gorilla/websocket"
)

type Move struct {
  Source string `json:"source"`
  Target string `json:"target"`
}

var conns []*websocket.Conn
var moves []Move

var upgrader = websocket.Upgrader{
  CheckOrigin: func(r *http.Request) bool { return true },
}

func play(w http.ResponseWriter, r *http.Request) {
  c, err := upgrader.Upgrade(w, r, nil)
  conns = append(conns, c)
  if err != nil {
    log.Print("upgrade:", err)
    return
  }
  defer c.Close()
  for {
    mt, message, err := c.ReadMessage()
    if err != nil {
      log.Println("read:", err)
      break
    }
    log.Printf("recv: %s", message)

    var move Move
    err = json.Unmarshal(message, &move)
    moves = append(moves, move)

    for _, conn := range conns {
      if conn != c {
        err = conn.WriteMessage(mt, message)
      }
    }

  }
}

func state(w http.ResponseWriter, r *http.Request) {
  w.Header().Set("Access-Control-Allow-Origin", "*")
  b, _ := json.Marshal(moves)
  fmt.Fprintf(w, "%s", string(b))
}

func main() {
  http.HandleFunc("/play", play)
  http.HandleFunc("/state", state)
  log.Fatal(http.ListenAndServe("localhost:2000", nil))
}
