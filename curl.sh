# Home
ip=10.0.0.21
port=8081
url=$ip:$port
curl $url

# About
curl $url/about

# Get students
curl $url/students \
-H "User-Role: Viewer"

# Get student
curl $url/student/eve@example.com

# POST student
curl -X POST $url/student \
-H "User-Role: Admin" \
-d "email=zach@example.com" \
-d "name=Zach Test" \
-d "course=MyCourse"

# PUT student
curl -X PUT $url/student \
-H "User-Role: Admin" \
-d "email=zach@example.com" \
-d "name=Zach Steinberg" \
-d "course=MyCourse"

# DELETE student
curl -X DELETE $url/student/zach@example.com \
-H "User-Role: Admin"
