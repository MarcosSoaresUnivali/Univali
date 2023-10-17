import streamlit as st
from streamlit_folium import st_folium
import folium

m = folium.Map(location=[-26.8898254,-49.1052913], zoom_start=16)
folium.Marker(
    [-26.8898254,-49.1052913],
    popup="Home",
    tooltip="Minha casa"
).add_to(m)

st_data = st_folium(m, width=725)

#########################################

from streamlit_extras.app_logo import add_logo

if st.checkbox("Use url", value=True):
    add_logo("http://placekitten.com/120/120")
else:
    add_logo("gallery/kitty.jpeg", height=300)
st.write("👈 Check out the cat in the nav-bar!")


##################################################

from streamlit_extras.badges import badge

badge(type="twitter", name="streamlit")


##################################################

from streamlit_extras.buy_me_a_coffee import button

button(username="fake-username", floating=True, width=221)

##################################################

from streamlit_card import card

card(
    title="Hello World!",
    text="Some description",
    image="http://placekitten.com/300/250",
    url="https://www.google.com",
)


###################################################

from streamlit_extras.colored_header import colored_header

colored_header(
    label="My New Pretty Colored Header",
    description="This is a description",
    color_name="violet-70",
)

