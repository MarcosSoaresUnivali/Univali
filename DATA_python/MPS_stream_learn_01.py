# ==================================================================================================
# MPS_stream_learn_01 - Experiment 01
# Marcos Soares - 29.set.2023
#
# Subject: TCC3 - Resolve the log SQL table checks importing data from CORP (Oracle)
# ==================================================================================================
#
import streamlit as st
import pyodbc

# Initialize connection.
# Uses st.cache_resource to only run once.
@st.cache_resource
def init_connection():
    return pyodbc.connect(
        "DRIVER={ODBC Driver 17 for SQL Server};SERVER="
        + st.secrets["server"]
        + ";DATABASE="
        + st.secrets["database"]
        + ";UID="
        + st.secrets["username"]
        + ";PWD="
        + st.secrets["password"]
    )

conn = init_connection()
