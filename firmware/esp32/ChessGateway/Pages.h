#ifndef HTMLPAGE_H
#define HTMLPAGE_H

#include <Arduino.h>

const char indexPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <link rel="icon" href="data:image/svg+xml,<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 100 100'><text y='.9em' font-size='90'>♟</text></svg>">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Chess Game</title>
  <link href="https://fonts.googleapis.com/css2?family=Playfair+Display:wght@400;700&family=Montserrat:wght@300;400;600&display=swap" rel="stylesheet">
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
    }
    
    :root {
      --theme-primary: #b58863;
      --theme-secondary: #f0d9b5;
      --theme-text: #2c2c2c;
      --theme-button-text: white;
      --container-bg: rgba(240, 217, 181, 0.9);
      --container-border: rgba(181, 136, 99, 0.3);
    }
    
    body {
      background-color: var(--theme-secondary);
      background-image: url('https://images.unsplash.com/photo-1541140134513-85a161dc4a00?ixlib=rb-1.2.1&auto=format&fit=crop&w=1350&q=80');
      background-size: cover;
      background-position: center;
      background-blend-mode: overlay;
      display: flex;
      flex-direction: column;
      justify-content: center;
      align-items: center;
      min-height: 100vh;
      color: var(--theme-text);
      transition: all 0.5s ease;
      padding: 20px;
    }
    
    .container {
      background-color: var(--container-bg);
      border-radius: 15px;
      padding: 3rem;
      box-shadow: 0 10px 30px rgba(0, 0, 0, 0.2);
      max-width: 800px;
      width: 100%;
      text-align: center;
      border: 1px solid var(--container-border);
      backdrop-filter: blur(5px);
      position: relative;
      overflow: hidden;
      transition: all 0.5s ease;
    }
    
    .container::before {
      content: "";
      position: absolute;
      top: 0;
      left: 0;
      right: 0;
      height: 8px;
      background: linear-gradient(90deg, var(--theme-primary), var(--theme-secondary), var(--theme-primary));
    }
    
    .title {
      font-family: 'Playfair Display', serif;
      font-size: 3.5rem;
      margin-bottom: 1.5rem;
      color: var(--theme-text);
      text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.1);
      letter-spacing: 2px;
      position: relative;
      display: inline-block;
    }
    
    .title::after {
      content: "";
      position: absolute;
      bottom: -10px;
      left: 50%;
      transform: translateX(-50%);
      width: 100px;
      height: 3px;
      background: linear-gradient(90deg, transparent, var(--theme-primary), transparent);
    }
    
    .subtitle {
      font-family: 'Playfair Display', serif;
      font-size: 1.2rem;
      font-style: italic;
      margin-bottom: 2.5rem;
      color: #555;
    }
    
    .menu {
      display: flex;
      flex-direction: column;
      gap: 1.5rem;
      width: 100%;
      max-width: 400px;
      margin: 0 auto;
    }
    
    .menu-btn, .option-btn, .close-modal-btn {
      padding: 1rem 1.5rem;
      font-size: 1.2rem;
      font-family: 'Montserrat', sans-serif;
      font-weight: 600;
      border: none;
      border-radius: 8px;
      background-color: var(--theme-primary);
      color: var(--theme-button-text);
      cursor: pointer;
      transition: all 0.3s ease;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.15);
      position: relative;
      overflow: hidden;
      letter-spacing: 1px;
    }
    
    .menu-btn::before {
      content: "";
      position: absolute;
      top: 0;
      left: -100%;
      width: 100%;
      height: 100%;
      background: linear-gradient(90deg, transparent, rgba(255,255,255,0.2), transparent);
      transition: 0.5s;
    }
    
    .menu-btn:hover, .option-btn:hover {
      transform: translateY(-3px);
      box-shadow: 0 8px 15px rgba(0, 0, 0, 0.2);
    }
    
    .menu-btn:hover::before {
      left: 100%;
    }
    
    .menu-btn:active, .option-btn:active {
      transform: translateY(0);
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.15);
    }
    
    .chess-piece-decoration {
      position: absolute;
      opacity: 0.1;
      font-size: 10rem;
      z-index: -1;
    }
    
    .piece-king {
      top: 20px;
      left: 20px;
    }
    
    .piece-queen {
      bottom: 20px;
      right: 20px;
    }
    
    .overlay {
      position: fixed;
      top: 0;
      left: 0;
      width: 100%;
      height: 100%;
      background-color: rgba(0, 0, 0, 0.8);
      display: none;
      justify-content: center;
      align-items: center;
      z-index: 100;
      backdrop-filter: blur(3px);
    }
    
    .rules-container {
      background-color: var(--theme-secondary);
      padding: 2.5rem;
      border-radius: 15px;
      max-width: 800px;
      max-height: 80vh;
      overflow-y: auto;
      position: relative;
      box-shadow: 0 0 30px rgba(0, 0, 0, 0.4);
      border: 1px solid rgba(181, 136, 99, 0.5);
    }
    
    .close-btn {
      position: absolute;
      top: 1rem;
      right: 1rem;
      font-size: 1.5rem;
      cursor: pointer;
      background: none;
      border: none;
      color: var(--theme-text);
      transition: transform 0.2s;
    }
    
    .close-btn:hover {
      transform: rotate(90deg);
    }
    
    .rules-title {
      margin-bottom: 1.5rem;
      color: var(--theme-text);
      font-family: 'Playfair Display', serif;
      font-size: 2rem;
      border-bottom: 2px solid var(--theme-primary);
      padding-bottom: 0.5rem;
      display: inline-block;
    }
    
    .rules-content {
      line-height: 1.8;
      color: var(--theme-text);
      font-family: 'Montserrat', sans-serif;
      text-align: left;
    }
    
    .rules-content p {
      margin-bottom: 1rem;
    }
    
    .rules-content strong {
      color: var(--theme-primary);
    }
    
    .rules-content ul {
      margin-left: 1.5rem;
      margin-bottom: 1rem;
    }
    
    /* Modal styling */
    .modal {
      display: none;
      flex-direction: column;
      gap: 1.5rem;
      background-color: var(--theme-secondary);
      padding: 2.5rem;
      border-radius: 15px;
      position: fixed;
      top: 50%;
      left: 50%;
      transform: translate(-50%, -50%);
      z-index: 100;
      box-shadow: 0 0 30px rgba(0, 0, 0, 0.4);
      max-width: 500px;
      width: 90%;
      border: 1px solid rgba(181, 136, 99, 0.5);
    }
    
    .modal h2 {
      font-family: 'Playfair Display', serif;
      color: var(--theme-text);
      margin-bottom: 1rem;
      border-bottom: 2px solid var(--theme-primary);
      padding-bottom: 0.5rem;
    }
    
    .close-modal-btn {
      background-color: var(--theme-text);
    }
    
    .close-modal-btn:hover {
      background-color: #555;
    }
    
    /* Animation classes */
    @keyframes fadeIn {
      from { opacity: 0; transform: translateY(20px); }
      to { opacity: 1; transform: translateY(0); }
    }
    
    .animate-fade-in {
      animation: fadeIn 0.6s ease-out forwards;
    }
    
    @media (max-width: 768px) {
      .container {
        padding: 2rem 1.5rem;
      }
      
      .title {
        font-size: 2.5rem;
      }
      
      .subtitle {
        font-size: 1rem;
      }
      
      .menu {
        max-width: 300px;
      }
      
      .rules-container, .modal {
        padding: 1.5rem;
        width: 95%;
      }
    }
    
    @media (max-width: 480px) {
      .title {
        font-size: 2rem;
      }
      
      .menu-btn, .option-btn {
        padding: 0.8rem;
        font-size: 1rem;
      }
    }
  </style>
</head>
<body>
  <div class="container animate-fade-in">
    <div class="chess-piece-decoration piece-king">♔</div>
    <div class="chess-piece-decoration piece-queen">♕</div>
    
    <h1 class="title">FYP Chess Game</h1>
    <p class="subtitle">"Outsmart, Outplay, Checkmate."</p>
    
    <div class="menu">
      <button class="menu-btn" id="play-btn">Join Game</button>
      <button class="menu-btn" id="about-btn">About Chess</button>
      <button class="menu-btn" id="options-btn">Game Options</button>
    </div>
  </div>
  
  <!-- Chess Rules Overlay -->
  <div class="overlay" id="rules-overlay">
    <div class="rules-container">
      <button class="close-btn" id="close-rules-btn">✕</button>
      <h2 class="rules-title">Chess Rules</h2>
      <div class="rules-content">
        <p><strong>1. The Board:</strong> Chess is played on an 8x8 grid with alternating light and dark squares.</p>
        <p><strong>2. The Pieces:</strong> Each player starts with 16 pieces: 1 king, 1 queen, 2 rooks, 2 bishops, 2 knights, and 8 pawns.</p>
        <p><strong>3. Movement:</strong>
          <ul>
            <li><strong>King:</strong> One square in any direction</li>
            <li><strong>Queen:</strong> Any number of squares in any direction</li>
            <li><strong>Rook:</strong> Any number of squares horizontally or vertically</li>
            <li><strong>Bishop:</strong> Any number of squares diagonally</li>
            <li><strong>Knight:</strong> L-shape (two squares in one direction, then one square perpendicular)</li>
            <li><strong>Pawn:</strong> Forward one square, captures diagonally</li>
          </ul>
        </p>
        <p><strong>4. Special Moves:</strong>
          <ul>
            <li><strong>Castling:</strong> King moves two squares toward a rook, which jumps over</li>
            <li><strong>En Passant:</strong> Special pawn capture under specific conditions</li>
            <li><strong>Promotion:</strong> Pawn reaching the last rank becomes any other piece</li>
          </ul>
        </p>
        <p><strong>5. Check:</strong> When a king is under immediate attack by an opponent's piece.</p>
        <p><strong>6. Checkmate:</strong> When a king is in check and cannot escape, ending the game.</p>
        <p><strong>7. Draw Conditions:</strong> Stalemate, insufficient material, threefold repetition, or 50-move rule.</p>
      </div>
    </div>
  </div>
  
  <!-- Color Options Modal -->
  <div class="modal" id="color-options">
    <h2>Choose Board Theme</h2>
    <button class="option-btn" id="classic-option">Traditional Elegance (Brown & Beige)</button>
    <button class="option-btn" id="green-option">Forest Focus (Green & Ivory)</button>
    <button class="option-btn" id="blue-option">Deep Calm (Blue & Silver)</button>
    <button class="close-modal-btn" id="close-color-btn">Close</button>
  </div>
  
  <script>
    // DOM Elements
    const playBtn = document.getElementById('play-btn');
    const aboutBtn = document.getElementById('about-btn');
    const optionsBtn = document.getElementById('options-btn');
    const rulesOverlay = document.getElementById('rules-overlay');
    const closeRulesBtn = document.getElementById('close-rules-btn');
    const colorOptions = document.getElementById('color-options');
    const closeColorBtn = document.getElementById('close-color-btn');
    const container = document.querySelector('.container');
    
    // Theme color definitions
    const themes = {
      classic: {
        primary: '#b58863',
        secondary: '#f0d9b5',
        text: '#2c2c2c',
        buttonText: 'white',
        containerBg: 'rgba(240, 217, 181, 0.9)',
        containerBorder: 'rgba(181, 136, 99, 0.3)'
      },
      green: {
        primary: '#6b8e23',
        secondary: '#e6e8c0',
        text: '#2c2c2c',
        buttonText: 'white',
        containerBg: 'rgba(230, 232, 192, 0.9)',
        containerBorder: 'rgba(107, 142, 35, 0.3)'
      },
      blue: {
        primary: '#4682b4',
        secondary: '#e6e8f0',
        text: '#2c2c2c',
        buttonText: 'white',
        containerBg: 'rgba(230, 232, 240, 0.9)',
        containerBorder: 'rgba(70, 130, 180, 0.3)'
      }
    };
    
    // Current selected theme
    let selectedTheme = localStorage.getItem('chessTheme') || 'classic';
    
    // Apply theme colors
    function applyTheme(theme) {
      const root = document.documentElement;
      const themeColors = themes[theme];
      
      root.style.setProperty('--theme-primary', themeColors.primary);
      root.style.setProperty('--theme-secondary', themeColors.secondary);
      root.style.setProperty('--theme-text', themeColors.text);
      root.style.setProperty('--theme-button-text', themeColors.buttonText);
      root.style.setProperty('--container-bg', themeColors.containerBg);
      root.style.setProperty('--container-border', themeColors.containerBorder);
      
      // Update the selected theme
      selectedTheme = theme;
      localStorage.setItem('chessTheme', theme);
    }
    
    // Start the game with selected options
    function startGame() {
      const url = `/game?theme=${encodeURIComponent(selectedTheme)}`;
      window.location.href = url;
    }
    
    // Animation on load
    document.addEventListener('DOMContentLoaded', () => {
      setTimeout(() => {
        container.classList.add('animate-fade-in');
      }, 100);
      
      // Load saved theme
      applyTheme(selectedTheme);
      
      // Set background based on theme
      document.body.style.backgroundColor = themes[selectedTheme].secondary;
      document.body.style.backgroundImage = 'url("https://images.unsplash.com/photo-1541140134513-85a161dc4a00?ixlib=rb-1.2.1&auto=format&fit=crop&w=1350&q=80")';
    });
    
    // Event Listeners with animations
    playBtn.addEventListener('click', startGame);
    
    aboutBtn.addEventListener('click', () => {
      rulesOverlay.style.display = 'flex';
      setTimeout(() => {
        document.querySelector('.rules-container').classList.add('animate-fade-in');
      }, 10);
    });
    
    optionsBtn.addEventListener('click', () => {
      colorOptions.style.display = 'flex';
      setTimeout(() => {
        colorOptions.classList.add('animate-fade-in');
      }, 10);
    });
    
    closeRulesBtn.addEventListener('click', () => {
      document.querySelector('.rules-container').classList.remove('animate-fade-in');
      setTimeout(() => {
        rulesOverlay.style.display = 'none';
      }, 300);
    });
    
    closeColorBtn.addEventListener('click', () => {
      colorOptions.classList.remove('animate-fade-in');
      setTimeout(() => {
        colorOptions.style.display = 'none';
      }, 300);
    });
    
    // Close modals when clicking outside
    window.addEventListener('click', (e) => {
      if (e.target === rulesOverlay) {
        document.querySelector('.rules-container').classList.remove('animate-fade-in');
        setTimeout(() => {
          rulesOverlay.style.display = 'none';
        }, 300);
      }
      if (e.target === colorOptions) {
        colorOptions.classList.remove('animate-fade-in');
        setTimeout(() => {
          colorOptions.style.display = 'none';
        }, 300);
      }
    });
    
    // Color selection functionality
    document.getElementById('classic-option').addEventListener('click', () => {
      applyTheme('classic');
      document.body.style.backgroundColor = themes.classic.secondary;
    });
    
    document.getElementById('green-option').addEventListener('click', () => {
      applyTheme('green');
      document.body.style.backgroundColor = themes.green.secondary;
    });
    
    document.getElementById('blue-option').addEventListener('click', () => {
      applyTheme('blue');
      document.body.style.backgroundColor = themes.blue.secondary;
    });
  </script>
</body>
</html>
)rawliteral";


const char gamePage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <link rel="icon" href="data:image/svg+xml,<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 100 100'><text y='.9em' font-size='90'>♟</text></svg>">
  <title>FYP Chessboard</title>
  <style>
    :root {
      --white-square: #f0d9b5;
      --black-square: #b58863;
      --white-piece: white;
      --black-piece: black;
      --active-timer-white: #f0d9b5;
      --active-timer-black: #b58863;
    }

    body {
      display: flex;
      flex-direction: column;
      justify-content: center;
      align-items: center;
      height: 100vh;
      background: #eee;
      font-family: sans-serif;
      gap: 20px;
      margin: 0;
      padding: 20px;
      box-sizing: border-box;
    }

    .title-container {
      text-align: center;
      margin-bottom: 20px;
      user-select: none;
    }

    .title-container h1 {
      font-family: 'Playfair Display', serif;
      font-size: 3rem;
      color: #2c2c2c;
      margin: 0;
      letter-spacing: 3px;
      text-shadow: 1px 1px 2px rgba(0,0,0,0.15);
    }

    .title-container p {
      font-family: 'Playfair Display', serif;
      font-size: 1.2rem;
      font-style: italic;
      color: #555;
      margin: 4px 0 0 0;
    }

    /* Container to hold timer, bench and board side by side */
    .game-container {
      display: flex;
      align-items: flex-start;
      gap: 20px;
    }

    /* Timer styling */
    .time-tracker {
      display: flex;
      flex-direction: column;
      gap: 20px;
      font-family: 'Playfair Display', serif;
      user-select: none;
      width: 150px;
    }

    .player-timer {
      padding: 10px;
      border-radius: 10px;
      color: #fff;
      text-align: center;
      box-shadow: 0 0 8px rgba(0,0,0,0.2);
      transition: background-color 0.3s ease;
    }

    .player-timer.active {
      box-shadow: 0 0 15px 3px gold;
    }

    .player-label {
      font-weight: bold;
      font-size: 1.1rem;
      margin-bottom: 6px;
    }

    .player-label.white {
      color: #fff;
      background-color: var(--black-square);
      border-radius: 6px;
      padding: 2px 6px;
    }

    .player-label.black {
      color: #fff;
      background-color: #403b35; /* dark gray */
      border-radius: 6px;
      padding: 2px 6px;
    }

    .time-display {
      font-size: 2rem;
      font-weight: 900;
      letter-spacing: 2px;
      font-variant-numeric: tabular-nums;
    }

    .player-timer.active.white {
      background-color: var(--white-square);
      color: #2c2c2c;
    }

    .player-timer.active.black {
      background-color: var(--black-square);
      color: var(--white-piece);
    }

    .turn-display {
      font-style: italic;
      font-size: 1rem;
      color: #333;
      text-align: center;
      font-weight: bold;
    }

    /* Container for bench and its title */
    .bench-container {
      display: flex;
      flex-direction: column;
      align-items: center;
      gap: 8px;
    }

    .bench-title {
      font-size: 16px;
      font-weight: bold;
      color: #333;
      user-select: none;
      margin: 0;
      font-style: italic;
    }

    .bench {
      display: grid;
      grid-template-columns: repeat(6, 40px);
      grid-auto-rows: 40px;
      gap: 4px;
      width: 300px;
      max-height: 440px;
      padding: 8px;
      background: #ddd;
      border-radius: 5px;
      box-shadow: 0 0 8px rgba(0,0,0,0.15);
      overflow-y: auto;
    }

    .board {
      display: grid;
      grid-template-columns: repeat(8, 60px);
      grid-template-rows: repeat(8, 60px);
      border: 2px solid #333;
      border-radius: 5px;
      overflow: hidden;
      user-select: none;
    }

    .square {
      position: relative;
      width: 60px;
      height: 60px;
      display: flex;
      align-items: center;
      justify-content: center;
      user-select: none;
      font-size: 14px;
      transition: background-color 0.3s ease;
    }

    /* Add border-radius only on the 4 corner squares */
    .square[data-row="0"][data-col="0"] {
      border-top-left-radius: 5px;
    }
    .square[data-row="0"][data-col="7"] {
      border-top-right-radius: 2px;
    }
    .square[data-row="7"][data-col="0"] {
      border-bottom-left-radius: 2px;
    }
    .square[data-row="7"][data-col="7"] {
      border-bottom-right-radius: 5px;
    }

    .white { background-color: var(--white-square); }
    .black { background-color: var(--black-square); }

    /* Highlight for drag source and target */
    .highlight {
      border: 3px solid yellow;
      background-color: transparent !important;
      transform: scale(0.9);
      box-sizing: border-box;
      transition: border-color 0.3s ease, transform 0.3s ease;
      z-index: 10;
    }

    /* Coordinate labels inside squares */
    .coord-file {
      position: absolute;
      bottom: 3px;
      right: 3px;
      font-size: 10px;
      color: rgba(44, 44, 44, 0.7);
      font-weight: bold;
      user-select: none;
      pointer-events: none;
    }

    .coord-rank {
      position: absolute;
      top: 3px;
      left: 3px;
      font-size: 10px;
      color: rgba(44, 44, 44, 0.7);
      font-weight: bold;
      user-select: none;
      pointer-events: none;
    }

    .piece {
      width: 40px;
      height: 40px;
      cursor: grab;
      z-index: 1;
      font-size: 40px;
      color: var(--black-piece);
    }

    /* White pieces will have white color */
    .piece[data-piece^="w_"], 
    .white-piece {
      color: var(--white-piece);
    }

    .promotion-container {
      margin-top: 15px;
      text-align: center;
    }

    .promotion {
      display: flex;
      flex-wrap: wrap;
      justify-content: center;
      gap: 8px;
      padding: 10px;
      background-color: #f0f0f0;
      border-radius: 8px;
      min-height: 60px;
    }

    .promotion img.piece {
      width: 40px;
      height: 40px;
      cursor: pointer;
    }
  </style>
  
</head>
 
<body>
  <div class="title-container">
    <h1>FYP Chessboard</h1>
    <p>“Master the Game, One Move at a Time”</p>
  </div>

  <div class="game-container">
    <div class="bench-container">
      <h2 class="bench-title">Captured Pieces</h2>
      <div class="bench" id="bench" title="Captured pieces"></div>

      <div class="promotion-container">
        <h2 class="bench-title">Promotion Pieces</h2>
        <div class="promotion" id="promotion" title="Promotion pieces">
          <span class="white-piece" data-code="w_q">♕</span>
          <span class="white-piece" data-code="w_r">♖</span>
          <span class="white-piece" data-code="w_b">♗</span>
          <span class="white-piece" data-code="w_n">♘</span>
          <span class="piece" data-code="b_q">♛</span>
          <span class="piece" data-code="b_r">♜</span>
          <span class="piece" data-code="b_b">♝</span>
          <span class="piece" data-code="b_n">♞</span>
        </div>
      </div>
    </div>

    <div class="board" id="board"></div>

    <div class="time-tracker">
      <div id="player1-timer" class="player-timer active white">
        <div class="player-label white">White</div>
        <div class="time-display" id="white-time">05:00</div>
      </div>
      <div id="player2-timer" class="player-timer black">
        <div class="player-label black">Black</div>
        <div class="time-display" id="black-time">05:00</div>
      </div>
      <div class="turn-display" id="turn-display">Turn: White</div>
    </div>
  </div>

  <script>
    // Get theme from URL parameters
    const urlParams = new URLSearchParams(window.location.search);
    const theme = urlParams.get('theme') || 'classic';
    
    // Player color is always white
    const playerColor = 'white';

    // Define color themes
    const themes = {
      classic: {
        whiteSquare: '#f0d9b5',
        blackSquare: '#b58863',
        whitePiece: 'white',
        blackPiece: 'black'
      },
      green: {
        whiteSquare: '#e6e8c0',
        blackSquare: '#6b8e23',
        whitePiece: 'white',
        blackPiece: 'black'
      },
      blue: {
        whiteSquare: '#e6e8f0',
        blackSquare: '#4682b4',
        whitePiece: 'white',
        blackPiece: 'black'
      }
    };

    // Apply selected theme
    function applyTheme(theme) {
      const root = document.documentElement;
      const themeColors = themes[theme] || themes.classic;
      
      root.style.setProperty('--white-square', themeColors.whiteSquare);
      root.style.setProperty('--black-square', themeColors.blackSquare);
      root.style.setProperty('--white-piece', themeColors.whitePiece);
      root.style.setProperty('--black-piece', themeColors.blackPiece);
      root.style.setProperty('--active-timer-white', themeColors.whiteSquare);
      root.style.setProperty('--active-timer-black', themeColors.blackSquare);
    }

    // Apply the theme on page load
    applyTheme(theme);

    const PIECES = {
      'w_p': '♙',
      'w_r': '♖',
      'w_n': '♘',
      'w_b': '♗',
      'w_q': '♕',
      'w_k': '♔',
      'b_p': '♟',
      'b_r': '♜',
      'b_n': '♞',
      'b_b': '♝',
      'b_q': '♛',
      'b_k': '♚'
    };

    let promotionPiecesAvailable = new Set([
      'w_q', 'w_r', 'w_b', 'w_n',
      'b_q', 'b_r', 'b_b', 'b_n'
    ]);

    const layout = [
      ['b_r', 'b_n', 'b_b', 'b_q', 'b_k', 'b_b', 'b_n', 'b_r'],
      ['b_p', 'b_p', 'b_p', 'b_p', 'b_p', 'b_p', 'b_p', 'b_p'],
      ['', '', '', '', '', '', '', ''],
      ['', '', '', '', '', '', '', ''],
      ['', '', '', '', '', '', '', ''],
      ['', '', '', '', '', '', '', ''],
      ['w_p', 'w_p', 'w_p', 'w_p', 'w_p', 'w_p', 'w_p', 'w_p'],
      ['w_r', 'w_n', 'w_b', 'w_q', 'w_k', 'w_b', 'w_n', 'w_r']
    ];

    const board = document.getElementById('board');
    const bench = document.getElementById('bench');
    const files = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'];

    let draggedPiece = null;
    let sourceSquare = null;
    let currentHoverSquare = null;

    // Store captured pieces here
    const capturedPieces = [];

    // Timer variables
    let whiteTime = 600; // 10 minutes
    let blackTime = 600; // 10 minutes
    let activePlayer = 'white'; // Always start with white's turn
    let timerInterval = null;

    let promotionPending = false;
    let promotionPosition = null; // {row, col}
    let promotionColor = null;

    // Enable promotion selection on promotion pieces
    const promotionContainer = document.getElementById('promotion');

    promotionContainer.addEventListener('click', (e) => {
      if (!promotionPending) return;

      const clicked = e.target;
      if (!clicked.classList.contains('piece')) return;

      const pieceCode = clicked.dataset.code;
      const pieceColor = pieceCode.startsWith('w') ? 'white' : 'black';

      // Only allow selecting piece of current promotion color
      if (pieceColor !== promotionColor) {
        return;
      }

      // Check if the piece is still available for promotion
      if (!promotionPiecesAvailable.has(pieceCode)) {
        return;
      }

      // Remove promoted piece from available set
      promotionPiecesAvailable.delete(pieceCode);

      // Replace pawn on layout with promoted piece
      if (promotionPosition) {
        layout[promotionPosition.row][promotionPosition.col] = pieceCode;
      }

      // Reset promotion state
      promotionPending = false;
      promotionPosition = null;
      promotionColor = null;

      // Update UI and game state
      renderBoard();
      renderBench();

      // Check for checkmate after promotion
      const opponentColor = activePlayer === 'white' ? 'black' : 'white';
      if (isCheckmate(opponentColor, layout)) {
        stopGame();
        return;
      }

      switchActivePlayer();
      updatePromotionDisplay(); // Hide or update promotion options
    });

    // Function to update promotion pieces display
    function updatePromotionDisplay() {
      const promotionDiv = document.getElementById('promotion');
      promotionDiv.innerHTML = ''; // clear current pieces

      const pieceOrder = [
        {code: 'w_q', emoji: '♕'},
        {code: 'w_r', emoji: '♖'},
        {code: 'w_b', emoji: '♗'},
        {code: 'w_n', emoji: '♘'},
        {code: 'b_q', emoji: '♛'},
        {code: 'b_r', emoji: '♜'},
        {code: 'b_b', emoji: '♝'},
        {code: 'b_n', emoji: '♞'},
      ];

      pieceOrder.forEach(({code, emoji}) => {
        if (promotionPiecesAvailable.has(code)) {
          const span = document.createElement('span');
          span.textContent = emoji;
          span.classList.add('piece');
          if (code.startsWith('w')) {
            span.classList.add('white-piece');
          }
          span.dataset.code = code;
          promotionDiv.appendChild(span);
        }
      });
    }

    function isKingInCheck(color, boardLayout) {
      // Find the king position
      let kingRow = -1, kingCol = -1;
      for (let r = 0; r < 8; r++) {
        for (let c = 0; c < 8; c++) {
          if (boardLayout[r][c] === `${color[0]}_k`) {
            kingRow = r;
            kingCol = c;
            break;
          }
        }
        if (kingRow !== -1) break;
      }
      if (kingRow === -1) return true; // No king found (invalid state)

      // Check if any enemy piece can attack the king
      const enemyColor = color === 'white' ? 'b' : 'w';
      for (let r = 0; r < 8; r++) {
        for (let c = 0; c < 8; c++) {
          const piece = boardLayout[r][c];
          if (piece && piece[0] === enemyColor) {
            if (isValidMove(piece, r, c, kingRow, kingCol, boardLayout, true)) {
              return true;
            }
          }
        }
      }
      return false;
    }

    function isCheckmate(color, boardLayout) {
      // First, check if king is in check
      if (!isKingInCheck(color, boardLayout)) return false;

      // Try all possible moves for this color
      for (let srcRow = 0; srcRow < 8; srcRow++) {
        for (let srcCol = 0; srcCol < 8; srcCol++) {
          const piece = boardLayout[srcRow][srcCol];
          if (piece && piece[0] === color[0]) {
            // Try all possible destinations for this piece
            for (let tgtRow = 0; tgtRow < 8; tgtRow++) {
              for (let tgtCol = 0; tgtCol < 8; tgtCol++) {
                // Check if move is valid (without check consideration)
                if (isValidMove(piece, srcRow, srcCol, tgtRow, tgtCol, boardLayout, true)) {
                  // Simulate the move
                  const tempBoard = boardLayout.map(row => row.slice());
                  tempBoard[tgtRow][tgtCol] = piece;
                  tempBoard[srcRow][srcCol] = '';
                  
                  // Check if king is still in check after this move
                  if (!isKingInCheck(color, tempBoard)) {
                    return false; // Found a legal move that escapes check
                  }
                }
              }
            }
          }
        }
      }
      return true; // No legal moves found to escape check
    }

    function stopGame() {
      // Stop the timer
      clearInterval(timerInterval);
      
      // Disable all drag events
      const pieces = document.querySelectorAll('.piece');
      pieces.forEach(piece => {
        piece.draggable = false;
        piece.style.pointerEvents = 'none';
      });

      // Disable square events
      const squares = document.querySelectorAll('.square');
      squares.forEach(square => {
        square.style.pointerEvents = 'none';
      });

      // Update turn display to show game over
      const turnDisplay = document.getElementById('turn-display');
      turnDisplay.textContent = 'Game Over!';
    }

    // Render the board squares and pieces
    function renderBoard() {
      board.innerHTML = '';

      for (let row = 0; row < 8; row++) {
        for (let col = 0; col < 8; col++) {
          const square = document.createElement('div');
          square.classList.add('square');
          square.dataset.row = row;
          square.dataset.col = col;
          square.classList.add((row + col) % 2 === 0 ? 'white' : 'black');

          // Add rank labels (numbers) on the left edge
          if (col === 0) {
            const rankLabel = document.createElement('div');
            rankLabel.textContent = 8 - row; 
            rankLabel.classList.add('coord-rank');
            square.appendChild(rankLabel);
          }

          if (row === 7) {
            const fileLabel = document.createElement('div');
            fileLabel.textContent = files[col]; 
            fileLabel.classList.add('coord-file');
            square.appendChild(fileLabel);
          }

          // Add piece emoji if present
          const pieceCode = layout[row][col];
          if (pieceCode) {
            const span = document.createElement('span');
            span.textContent = PIECES[pieceCode];
            span.classList.add('piece');
            if (pieceCode.startsWith('w')) {
              span.classList.add('white-piece');
            }
            span.draggable = true;
            span.dataset.row = row;
            span.dataset.col = col;
            span.dataset.piece = pieceCode;

            // Drag events
            span.addEventListener('dragstart', dragStart);
            span.addEventListener('dragend', dragEnd);
            square.appendChild(span);
          }

          // Drag and drop events on squares
          square.addEventListener('dragover', dragOver);
          square.addEventListener('drop', dropPiece);
          square.addEventListener('dragenter', dragEnter);
          square.addEventListener('dragleave', dragLeave);

          board.appendChild(square);
        }
      }
    }

    // Render captured pieces in bench
    function renderBench() {
      bench.innerHTML = '';
      capturedPieces.forEach(code => {
        const span = document.createElement('span');
        span.textContent = PIECES[code];
        span.classList.add('piece');
        if (code.startsWith('w')) {
          span.classList.add('white-piece');
        }
        bench.appendChild(span);
      });
    }

    // Drag handlers
    function dragStart(e) {
      const pieceCode = e.target.dataset.piece;
      const pieceColor = pieceCode.startsWith('w') ? 'white' : 'black';
      
      // Only allow dragging if:
      // 1. It's the player's piece (white)
      // 2. It's that player's turn (matches activePlayer)
      if (pieceColor !== playerColor || pieceColor !== activePlayer) {
        e.preventDefault();
        return false;
      }

      draggedPiece = e.target;
      sourceSquare = draggedPiece.parentElement;
      setTimeout(() => {
        draggedPiece.style.display = 'none';
      }, 0);
      
      return true;
    }

    function dragEnd(e) {
      draggedPiece.style.display = 'block';
      draggedPiece = null;
      sourceSquare = null;
      clearHighlights();
    }

    function dragOver(e) {
      e.preventDefault();
    }

    function dragEnter(e) {
      if (e.target.classList.contains('square')) {
        currentHoverSquare = e.target;
        e.target.classList.add('highlight');
      }
    }

    function dragLeave(e) {
      if (e.target.classList.contains('square')) {
        e.target.classList.remove('highlight');
      }
    }

    // Clear all highlights
    function clearHighlights() {
      const squares = document.querySelectorAll('.square');
      squares.forEach(sq => {
        sq.classList.remove('highlight');
      });
    }

    // Helper: Check if position is inside board
    function isInsideBoard(r, c) {
      return r >= 0 && r < 8 && c >= 0 && c < 8;
    }

    // Helper: Check if path is clear between source and target (for sliding pieces)
    function isPathClear(srcRow, srcCol, tgtRow, tgtCol, boardLayout) {
      const dr = Math.sign(tgtRow - srcRow);
      const dc = Math.sign(tgtCol - srcCol);

      let r = srcRow + dr;
      let c = srcCol + dc;
      while (r !== tgtRow || c !== tgtCol) {
        if (boardLayout[r][c] !== '') return false;
        r += dr;
        c += dc;
      }
      return true;
    }

    // Check if a square is under attack by opponent
    function isSquareUnderAttack(color, row, col, boardLayout) {
      const enemyColor = color === 'white' ? 'b' : 'w';
      
      for (let r = 0; r < 8; r++) {
        for (let c = 0; c < 8; c++) {
          const piece = boardLayout[r][c];
          if (piece && piece[0] === enemyColor) {
            if (isValidMove(piece, r, c, row, col, boardLayout, true)) {
              return true;
            }
          }
        }
      }
      return false;
    }

    // Validate moves based on piece type and board state
    function isValidMove(pieceCode, srcRow, srcCol, tgtRow, tgtCol, boardLayout = layout, ignoreCheck = false) {
      const color = pieceCode[0]; // 'w' or 'b'
      const type = pieceCode[2]; // 'p', 'r', 'n', 'b', 'q', 'k'

      const targetPiece = boardLayout[tgtRow][tgtCol];
      const targetColor = targetPiece ? targetPiece[0] : null;

      // Cannot capture own piece
      if (targetColor === color) return false;

      const dr = tgtRow - srcRow;
      const dc = tgtCol - srcCol;

      switch (type) {
        case 'p': // Pawn
          const direction = color === 'w' ? -1 : 1;
          const startRow = color === 'w' ? 6 : 1;

          // Normal move forward by 1
          if (dc === 0 && dr === direction && targetPiece === '') {
            return true;
          }
          // Initial double move forward by 2
          if (dc === 0 && dr === 2 * direction && srcRow === startRow && 
              targetPiece === '' && boardLayout[srcRow + direction][srcCol] === '') {
            return true;
          }
          // Capture move diagonally by 1
          if (Math.abs(dc) === 1 && dr === direction && targetPiece !== '' && targetColor !== color) {
            return true;
          }
          return false;

        case 'r': // Rook
          if (dr !== 0 && dc !== 0) return false;
          return isPathClear(srcRow, srcCol, tgtRow, tgtCol, boardLayout);

        case 'n': // Knight
          return (Math.abs(dr) === 2 && Math.abs(dc) === 1) || 
                (Math.abs(dr) === 1 && Math.abs(dc) === 2);

        case 'b': // Bishop
          if (Math.abs(dr) !== Math.abs(dc)) return false;
          return isPathClear(srcRow, srcCol, tgtRow, tgtCol, boardLayout);

        case 'q': // Queen
          if (dr === 0 || dc === 0 || Math.abs(dr) === Math.abs(dc)) {
            return isPathClear(srcRow, srcCol, tgtRow, tgtCol, boardLayout);
          }
          return false;

        case 'k': // King
          // Normal king move
          return Math.abs(dr) <= 1 && Math.abs(dc) <= 1;

        default:
          return false;
      }
    }

function dropPiece(e) {
  e.preventDefault();
  clearHighlights();

  if (promotionPending) {
    return;
  }

  if (!draggedPiece) return;

  const targetSquare = e.target.closest('.square');
  if (!targetSquare) return;

  const srcRow = parseInt(draggedPiece.dataset.row);
  const srcCol = parseInt(draggedPiece.dataset.col);
  const tgtRow = parseInt(targetSquare.dataset.row);
  const tgtCol = parseInt(targetSquare.dataset.col);

  // Prevent move if same square
  if (srcRow === tgtRow && srcCol === tgtCol) return;

  const pieceCode = draggedPiece.dataset.piece;
  const pieceColor = pieceCode.startsWith('w') ? 'white' : 'black';

  // Only allow moving the player's pieces on their turn
  if (pieceColor !== playerColor || pieceColor !== activePlayer) {
    return;
  }

  const targetPieceCode = layout[tgtRow][tgtCol];

  // Convert coordinates to chess notation (a1, h8, etc.)
  const srcNotation = files[srcCol] + (8 - srcRow);
  const tgtNotation = files[tgtCol] + (8 - tgtRow);
  const moveNotation = srcNotation + tgtNotation;

  // Check if the move is valid according to piece rules
  if (!isValidMove(pieceCode, srcRow, srcCol, tgtRow, tgtCol, layout, true)) {
    return;
  }

  // Create a temporary board to test the move
  const tempBoard = layout.map(row => row.slice());
  tempBoard[tgtRow][tgtCol] = pieceCode;
  tempBoard[srcRow][srcCol] = '';

  // Check if this move would leave the king in check
  if (isKingInCheck(playerColor, tempBoard)) {
    return;
  }

  // Capture if enemy piece present
  if (targetPieceCode) {
    capturedPieces.push(targetPieceCode);
  }

  // Execute the move
  layout[tgtRow][tgtCol] = pieceCode;
  layout[srcRow][srcCol] = '';

  // Send move to server
  sendMoveToServer(moveNotation);

  // Check for promotion eligibility
  const isPawn = pieceCode[2] === 'p';
  const promotionRow = playerColor === 'white' ? 0 : 7;

  if (isPawn && tgtRow === promotionRow) {
    promotionPending = true;
    promotionPosition = { row: tgtRow, col: tgtCol };
    promotionColor = playerColor;
    renderBoard();
    renderBench();
    return;
  }

  renderBoard();
  renderBench();

  // Check for checkmate after the move
  const opponentColor = playerColor === 'white' ? 'black' : 'white';
  if (isCheckmate(opponentColor, layout)) {
    stopGame();
    return;
  }

  switchActivePlayer();
}

    function isStalemate(color, boardLayout) {
      // King is not in check but has no legal moves
      if (isKingInCheck(color, boardLayout)) return false;

      // Check if any piece has any legal move
      for (let srcRow = 0; srcRow < 8; srcRow++) {
        for (let srcCol = 0; srcCol < 8; srcCol++) {
          const piece = boardLayout[srcRow][srcCol];
          if (piece && piece[0] === color[0]) {
            for (let tgtRow = 0; tgtRow < 8; tgtRow++) {
              for (let tgtCol = 0; tgtCol < 8; tgtCol++) {
                if (isValidMove(piece, srcRow, srcCol, tgtRow, tgtCol, boardLayout, true)) {
                  const tempBoard = boardLayout.map(row => row.slice());
                  tempBoard[tgtRow][tgtCol] = piece;
                  tempBoard[srcRow][srcCol] = '';
                  if (!isKingInCheck(color, tempBoard)) {
                    return false; // Found at least one legal move
                  }
                }
              }
            }
          }
        }
      }
      return true; // No legal moves found
    }

    function switchActivePlayer() {
      activePlayer = activePlayer === 'white' ? 'black' : 'white';
      setActivePlayer(activePlayer);
      startTimer(); // Restart the timer for the new active player
    }
    function setActivePlayer(player) {
      const whiteTimer = document.getElementById('player1-timer');
      const blackTimer = document.getElementById('player2-timer');
      const turnDisplay = document.getElementById('turn-display');

      if (player === 'white') {
        whiteTimer.classList.add('active', 'white');
        whiteTimer.classList.remove('black');
        blackTimer.classList.remove('active', 'black', 'white');
        turnDisplay.textContent = 'Turn: White';
      } else {
        blackTimer.classList.add('active', 'black');
        blackTimer.classList.remove('white');
        whiteTimer.classList.remove('active', 'white', 'black');
        turnDisplay.textContent = 'Turn: Black';
      }
    }

   // Update timer display every second
  function startTimer() {
    if (timerInterval) clearInterval(timerInterval);
    
    // Update the display immediately when starting
    document.getElementById('white-time').textContent = formatTime(whiteTime);
    document.getElementById('black-time').textContent = formatTime(blackTime);
    
    timerInterval = setInterval(() => {
      if (activePlayer === 'white') {
        whiteTime--;
        if (whiteTime < 0) whiteTime = 0;
        document.getElementById('white-time').textContent = formatTime(whiteTime);
      } else {
        blackTime--;
        if (blackTime < 0) blackTime = 0;
        document.getElementById('black-time').textContent = formatTime(blackTime);
      }
      
      // Check if either player has run out of time
      if (whiteTime === 0 || blackTime === 0) {
        stopGame();
      }
    }, 1000);
  }

  // Helper function to format time as MM:SS
  function formatTime(seconds) {
    const mins = Math.floor(seconds / 60);
    const secs = seconds % 60;
    return `${mins.toString().padStart(2, '0')}:${secs.toString().padStart(2, '0')}`;
  }

    function sendMoveToServer(move) {
      fetch('/move', {  
          method: 'POST',
          headers: {
              'Content-Type': 'application/x-www-form-urlencoded',
          },
          body: 'move=' + encodeURIComponent(move)
      })
      .then(response => response.text())
      .then(data => {
          // You can use this space if you want to handle the server response silently
      });
    }

    // Function to process a move received from the server
    function processReceivedMove(moveNotation) {
      if (!moveNotation || moveNotation.length < 4) return;
      
      // Convert notation to coordinates (e.g., "e2e4" -> [6,4] to [4,4])
      const srcCol = files.indexOf(moveNotation[0]);
      const srcRow = 8 - parseInt(moveNotation[1]);
      const tgtCol = files.indexOf(moveNotation[2]);
      const tgtRow = 8 - parseInt(moveNotation[3]);
      
      // Validate coordinates
      if (srcCol === -1 || srcRow < 0 || srcRow > 7 || 
          tgtCol === -1 || tgtRow < 0 || tgtRow > 7) {
          return;
      }
      
      const pieceCode = layout[srcRow][srcCol];
      if (!pieceCode) return;
      
      const pieceColor = pieceCode.startsWith('w') ? 'white' : 'black';
      
      // Only process moves for the opponent (black)
      if (pieceColor === playerColor) return;
      
      // Check if the move is valid
      if (!isValidMove(pieceCode, srcRow, srcCol, tgtRow, tgtCol, layout, true)) {
          return;
      }
      
      // Capture if enemy piece present
      const targetPieceCode = layout[tgtRow][tgtCol];
      if (targetPieceCode) {
          capturedPieces.push(targetPieceCode);
      }
      
      // Execute the move
      layout[tgtRow][tgtCol] = pieceCode;
      layout[srcRow][srcCol] = '';
      
      // Check for promotion
      const isPawn = pieceCode[2] === 'p';
      const promotionRow = pieceColor === 'white' ? 0 : 7;
      
      if (isPawn && tgtRow === promotionRow) {
          // Auto-promote to queen for received moves
          layout[tgtRow][tgtCol] = pieceColor === 'white' ? 'w_q' : 'b_q';
      }
      
      renderBoard();
      renderBench();
      
      // Check for checkmate
      if (isCheckmate(playerColor, layout)) {
          stopGame();
          return;
      }
      
      switchActivePlayer();
    }

// Poll the server for new moves
function pollForMoves() {
    fetch('/getmove')
        .then(response => {
            if (response.status === 200) {
                return response.text();
            }
            return null;
        })
        .then(move => {
            if (move) {
                processReceivedMove(move);
            }
            // Continue polling
            setTimeout(pollForMoves, 1000);
        })
        .catch(error => {
            console.error('Error polling for moves:', error);
            // Continue polling even if there's an error
            setTimeout(pollForMoves, 1000);
        });
}

// Start polling when the page loads
document.addEventListener('DOMContentLoaded', () => {
    // Start the timer
    startTimer();
    
    // Start polling for opponent moves
    pollForMoves();
});

    // Initial render and start timer
    renderBoard();
    renderBench();
    setActivePlayer(activePlayer);
    startTimer();
    updatePromotionDisplay();
</script>
  
</body> 
</html>
)rawliteral";

#endif