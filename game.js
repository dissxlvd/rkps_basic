// Game constants
const BOARD_SIZE = 8;
const PLAYER = "X";
const COMPUTER = "O";
const EMPTY = "-";

// Game state
let board = [];
let columnHeights = [];
let gameActive = false;
let playerTurn = true;
let gameOver = false;

// Heuristic values (from original BASIC game)
const V = [
	1, 100, 500, 1000000, 1, 800, 4000, 1000000, 1, 75, 900, 1000000000, 1, 450,
	3000, 1000000000,
];

// Initialize game
function initGame() {
	board = Array(BOARD_SIZE)
		.fill()
		.map(() => Array(BOARD_SIZE).fill(EMPTY));
	columnHeights = Array(BOARD_SIZE).fill(0);
	gameActive = true;
	gameOver = false;

	// Ask who goes first
	const goFirst = confirm(
		"Do you want to go first? (OK for Yes, Cancel for No)"
	);
	playerTurn = goFirst;

	updateDisplay();

	if (!goFirst) {
		setTimeout(computerMove, 500);
	}
}

// Update the display
function updateDisplay() {
	const boardElement = document.getElementById("board");
	boardElement.innerHTML = "";

	// Create board from top to bottom (row 7 to 0)
	for (let row = BOARD_SIZE - 1; row >= 0; row--) {
		const rowElement = document.createElement("div");
		rowElement.className = "row";

		for (let col = 0; col < BOARD_SIZE; col++) {
			const cell = document.createElement("div");
			cell.className = "cell";

			const piece = board[row][col];
			if (piece === PLAYER) {
				cell.classList.add("player");
				cell.textContent = "X";
			} else if (piece === COMPUTER) {
				cell.classList.add("computer");
				cell.textContent = "O";
			} else {
				cell.classList.add("empty");
				cell.textContent = "";
			}

			rowElement.appendChild(cell);
		}

		boardElement.appendChild(rowElement);
	}

	// Update column selector
	const selector = document.querySelector(".column-selector");
	selector.innerHTML = "";

	for (let i = 1; i <= BOARD_SIZE; i++) {
		const btn = document.createElement("button");
		btn.className = "column-btn";
		btn.textContent = i;
		btn.onclick = () => playerMove(i - 1);
		btn.disabled =
			!playerTurn || gameOver || columnHeights[i - 1] >= BOARD_SIZE;
		selector.appendChild(btn);
	}

	// Update status
	const status = document.getElementById("status");
	const turn = document.getElementById("turn");

	if (gameOver) {
		status.textContent = gameState.message;
		turn.textContent = "";
	} else {
		status.textContent = "Game in progress";
		turn.textContent = playerTurn ? "Your turn (X)" : "Computer's turn (O)...";
	}
}

// Player move
function playerMove(column) {
	if (!gameActive || !playerTurn || gameOver) return;

	if (columnHeights[column] >= BOARD_SIZE) {
		alert("Column is full! Choose another column.");
		return;
	}

	const row = columnHeights[column];
	board[row][column] = PLAYER;
	columnHeights[column]++;

	// Check for win
	if (checkWin(column, row, PLAYER)) {
		gameOver = true;
		playerTurn = false;
		updateDisplay();
		alert("YOU WIN!!!");
		return;
	}

	playerTurn = false;
	updateDisplay();

	// Computer move after delay
	setTimeout(computerMove, 500);
}

// Computer move AI
function computerMove() {
	if (!gameActive || playerTurn || gameOver) return;

	let bestColumn = -1;
	let bestValue = -Infinity;

	// Try each column
	for (let col = 0; col < BOARD_SIZE; col++) {
		if (columnHeights[col] >= BOARD_SIZE) continue;

		const row = columnHeights[col];
		let value = 1;

		// Simulate placing piece
		board[row][col] = COMPUTER;

		// Check for immediate win
		if (checkWin(col, row, COMPUTER)) {
			bestColumn = col;
			board[row][col] = EMPTY;
			break;
		}

		// Evaluate position (simplified heuristic)
		value += evaluatePosition(col, row, COMPUTER);

		// Check if blocking opponent
		board[row][col] = EMPTY;
		board[row][col] = PLAYER;
		if (checkWin(col, row, PLAYER)) {
			value = 1000; // High priority to block
		}
		board[row][col] = EMPTY;

		if (value > bestValue) {
			bestValue = value;
			bestColumn = col;
		}
	}

	// If no good move found, pick first available
	if (bestColumn === -1) {
		for (let col = 0; col < BOARD_SIZE; col++) {
			if (columnHeights[col] < BOARD_SIZE) {
				bestColumn = col;
				break;
			}
		}
	}

	if (bestColumn === -1) {
		// Tie game
		gameOver = true;
		updateDisplay();
		alert("TIE GAME...");
		return;
	}

	// Make the move
	const row = columnHeights[bestColumn];
	board[row][bestColumn] = COMPUTER;
	columnHeights[bestColumn]++;

	document.getElementById("status").textContent = `Computer picks column ${
		bestColumn + 1
	}`;

	// Check for win
	if (checkWin(bestColumn, row, COMPUTER)) {
		gameOver = true;
		updateDisplay();
		setTimeout(() => alert("COMPUTER WINS!!!"), 100);
		return;
	}

	playerTurn = true;
	updateDisplay();
}

// Simplified evaluation function
function evaluatePosition(col, row, player) {
	let score = 0;

	// Check horizontal
	let horizontal = 1;
	for (let i = 1; i < 4; i++) {
		if (col + i < BOARD_SIZE && board[row][col + i] === player) horizontal++;
		if (col - i >= 0 && board[row][col - i] === player) horizontal++;
	}

	// Check vertical
	let vertical = 1;
	for (let i = 1; i < 4; i++) {
		if (row + i < BOARD_SIZE && board[row + i][col] === player) vertical++;
		if (row - i >= 0 && board[row - i][col] === player) vertical++;
	}

	// Check diagonal \
	let diag1 = 1;
	for (let i = 1; i < 4; i++) {
		if (
			row + i < BOARD_SIZE &&
			col + i < BOARD_SIZE &&
			board[row + i][col + i] === player
		)
			diag1++;
		if (row - i >= 0 && col - i >= 0 && board[row - i][col - i] === player)
			diag1++;
	}

	// Check diagonal /
	let diag2 = 1;
	for (let i = 1; i < 4; i++) {
		if (
			row + i < BOARD_SIZE &&
			col - i >= 0 &&
			board[row + i][col - i] === player
		)
			diag2++;
		if (
			row - i >= 0 &&
			col + i < BOARD_SIZE &&
			board[row - i][col + i] === player
		)
			diag2++;
	}

	score = Math.max(horizontal, vertical, diag1, diag2);

	// Add bonus for center columns
	if (col >= 2 && col <= 5) score += 2;

	return score;
}

// Check for win
function checkWin(col, row, player) {
	// Check horizontal
	let count = 0;
	for (let c = 0; c < BOARD_SIZE; c++) {
		count = board[row][c] === player ? count + 1 : 0;
		if (count >= 4) return true;
	}

	// Check vertical
	count = 0;
	for (let r = 0; r < BOARD_SIZE; r++) {
		count = board[r][col] === player ? count + 1 : 0;
		if (count >= 4) return true;
	}

	// Check diagonal \
	count = 0;
	let startRow = row - Math.min(row, col);
	let startCol = col - Math.min(row, col);

	for (let i = 0; i < BOARD_SIZE; i++) {
		const r = startRow + i;
		const c = startCol + i;
		if (r >= BOARD_SIZE || c >= BOARD_SIZE) break;
		count = board[r][c] === player ? count + 1 : 0;
		if (count >= 4) return true;
	}

	// Check diagonal /
	count = 0;
	startRow = row + Math.min(BOARD_SIZE - 1 - row, col);
	startCol = col - Math.min(BOARD_SIZE - 1 - row, col);

	for (let i = 0; i < BOARD_SIZE; i++) {
		const r = startRow - i;
		const c = startCol + i;
		if (r < 0 || c >= BOARD_SIZE) break;
		count = board[r][c] === player ? count + 1 : 0;
		if (count >= 4) return true;
	}

	return false;
}

// UI Functions
function newGame() {
	const showInstr = confirm("Do you want to see instructions?");
	if (showInstr) {
		showInstructions();
	}
	initGame();
}

function showInstructions() {
	document.getElementById("instructions").style.display = "block";
}

function hideInstructions() {
	document.getElementById("instructions").style.display = "none";
}

// Initialize on load
window.onload = function () {
	newGame();
};
