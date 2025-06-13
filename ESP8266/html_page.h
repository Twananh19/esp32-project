#ifndef WEB_HTML_H
#define WEB_HTML_H

const char* html_page = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1" charset="utf-8">
    <style>
      button {
        width:200px;
        height:60px;
        font-size:28px;
        margin: 10px;
      }
      #status, #cardinfo {
        font-size:20px;
        margin-top:20px;
        color: green;
      }
    </style>
  </head>
  <body>
    <h1>Điều khiển cổng & Thông tin thẻ</h1>
    <div id="cardinfo">
      Đang chờ quẹt thẻ...
    </div>
    <button onclick="openGate('IN')">Mở cổng vào</button>
    <button onclick="openGate('OUT')">Mở cổng ra</button>
    // <button onclick="openGate('CLOSE')">Đóng cổng</button>
    <button id="registerBtn" style="display:none;" onclick="registerCard()">Đăng ký & mở cổng</button>
    <button onclick="showListCards()">Xem danh sách thẻ</button>
    <div id="listcards" style="display:none;"></div>

    <p id="status"></p>

    <script>
      // Hàm lấy thông tin thẻ mới nhất từ server
      function fetchCardInfo() {
        fetch('/cardinfo')
          .then(response => response.json())
          .then(data => {
            if(data && data.uuid) {
                if(data.isNew) {
                    document.getElementById('cardinfo').innerHTML =
                        '<span style="color:red;">Phát hiện UUID mới: ' + data.uuid + '</span><br>' +
                        '<b>Hãy nhấn nút để đăng ký và mở cổng!</b>';
                    document.getElementById('registerBtn').style.display = 'inline-block';
                } else {
                    document.getElementById('cardinfo').innerHTML =
                        'UID: ' + data.uuid + ' đang qua cổng';
                    document.getElementById('registerBtn').style.display = 'none';
                }
            } else {
                document.getElementById('cardinfo').innerText = 'Đang chờ quẹt thẻ...';
                document.getElementById('registerBtn').style.display = 'none';
            }
          })
          .catch(err => {
            document.getElementById('cardinfo').innerText = 'Không lấy được thông tin thẻ!';
            document.getElementById('registerBtn').style.display = 'none';
          });
      }

      function registerCard() {
          fetch('/register')
            .then(response => response.text())
            .then(data => {
              alert(data);
              fetchCardInfo();
            });
      }

      function showListCards() {
          fetch('/listcards')
            .then(response => response.json())
            .then(list => {
              let html = '<table border="1"><tr><th>UUID</th><th>Money</th><th>Last Time</th></tr>';
              list.forEach(card => {
                  html += `<tr><td>${card.uuid}</td><td>${card.money}</td><td>${card.time}</td></tr>`;
              });
              html += '</table>';
              document.getElementById('listcards').innerHTML = html;
              document.getElementById('listcards').style.display = 'block';
            });
      }

      // Gửi lệnh mở cổng qua server (server sẽ gửi UART cho ESP32)
      function openGate(type) {
        fetch('/gate/' + type)
          .then(response => response.text())
          .then(data => {
            document.getElementById('status').innerText = data;
          })
          .catch(err => {
            document.getElementById('status').innerText = 'Lỗi gửi lệnh!';
          });
      }

      setInterval(fetchCardInfo, 2000); // Cập nhật thông tin thẻ mỗi 2 giây
      fetchCardInfo();
    </script>
  </body>
</html>
)rawliteral";

#endif