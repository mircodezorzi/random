var board = null
var game = new Chess()
var $status = $('#status')
var $fen = $('#fen')
var $pgn = $('#pgn')

const side = 'white';
const p = side == 'black' ? 'w' : 'b'

const url='http://localhost:2000/state';

fetch(url)
  .then(function(response) {
    return response.json();
  })
  .then(function(response) {
    if (response) {
      for (move of response) {
        onDrop(move.source, move.target)
      }
      updateStatus()
      board.position(game.fen())
    }
  });


var s = new WebSocket("ws://localhost:2000/play");
s.onclose = () => {
  console.log('Web Socket Connection Closed');
};

s.onmessage = (res) => {

  var move = game.move({
    from: JSON.parse(res.data).source,
    to: JSON.parse(res.data).target,
    promotion: 'q'
  })

  if (move === null) return 'snapback'

  updateStatus()
  board.position(game.fen())
}

function onDragStart (source, piece, position, orientation) {
  // do not pick up pieces if the game is over
  if (game.game_over()) return false

  // only pick up pieces for the side to move
  re = new RegExp("^" + p)
  if ((game.turn() === p || piece.search(re) !== -1)) {
  // if ((game.turn() === 'w' && piece.search(/^b/) !== -1) ||
  //     (game.turn() === 'b' && piece.search(/^w/) !== -1)) {
    return false
  }
}

function onDrop (source, target) {
  var move = game.move({
    from: source,
    to: target,
    promotion: 'q'
  })

  s.send(JSON.stringify({
    source: source,
    target: target
  }));


  // illegal move
  if (move === null) return 'snapback'
  updateStatus()
}

// update the board position after the piece snap
// for castling, en passant, pawn promotion
function onSnapEnd () {
  board.position(game.fen())
}

function updateStatus () {
  var status = ''

  var moveColor = 'White'
  if (game.turn() === 'b') {
    moveColor = 'Black'
  }

  // checkmate?
  if (game.in_checkmate()) {
    status = 'Game over, ' + moveColor + ' is in checkmate.'
  }

  // draw?
  else if (game.in_draw()) {
    status = 'Game over, drawn position'
  }

  // game still on
  else {
    status = moveColor + ' to move'

    // check?
    if (game.in_check()) {
      status += ', ' + moveColor + ' is in check'
    }
  }

  $status.html(status)
  $fen.html(game.fen())
  $pgn.html(game.pgn())
}

var config = {
  draggable: true,
  position: 'start',
  onDragStart: onDragStart,
  onDrop: onDrop,
  onSnapEnd: onSnapEnd,
  orientation: side,
}


board = Chessboard('board1', config)

updateStatus()
