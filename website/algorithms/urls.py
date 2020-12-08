from django.conf.urls import url
from . import views

urlpatterns = [
    url(r'^$', views.index, name='index'),

    # /algorithms/123/
    url(r'^(?P<algorithm_id>[0-9]+)/$', views.detail, name='detail')
]